#include "ctpcapi.h"
 
#include "io.h"
#include "mongoapi.h"
#include "threadfunc.h"
#include "verbose.h"

#include <stdio.h>
#include <pthread.h>
#include <mongoc.h>

int main(int argc, char **argv) {
	setbuf(stdout, (char *) 0);
	char *file;
	if (argc == 1) {
		file = "others/xxx_2";
	}
	else if (argc == 2) {
		file = argv[1];
	}
	else {
		isError("wrong args");
	}
	char *InstrumentIDs[10]={NULL};
	int InstrumentNum = 1;
	char *mdlogfilepath, *tdlogfilepath, *mdserver, *tdserver, *mongodb_url_port, *BrokerID, *InvestorID, *UserID, *pd, *UserProductInfo;
	readinfo(file, &mdlogfilepath, &tdlogfilepath, &mdserver, &tdserver, &mongodb_url_port, &BrokerID, &InvestorID, &UserID, &pd, &UserProductInfo, &InstrumentIDs, &InstrumentNum);

	void *md = MD_create(mdlogfilepath, mdserver, BrokerID, UserID, pd, InstrumentIDs, InstrumentNum);
	void *td = TD_create(tdlogfilepath, tdserver, BrokerID, InvestorID, UserID, pd, UserProductInfo, THOST_TERT_RESUME); 

	mongoc_client_t *client = MongoAPI_create_client(mongodb_url_port);
	mongoc_collection_t **mcollections = MongoAPI_glue_collections(client, InstrumentIDs, InstrumentNum, BrokerID, UserID);

	struct BAR *bar = MongoAPI_fetch_1mbar(mcollections[0], 20140919, 20140922);
	int i;
	for (i = 0; i < bar->barsNum; ++i) {
		printf("i:%05d, nN: %d, bN: %d, InID: %s, beginYMD: %06d, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", i, bar->type, bar->barsNum, bar->InstrumentID, bar->beginYMD, bar->YMD[i], bar->btimeHMS[i], bar->etimeHMS[i], bar->openPrice[i], bar->closePrice[i], bar->uplimitPrice[i], bar->lowlimitPrice[i], bar->volume[i]);
	}
	free(bar);

	int running = 1;
	struct ThreadIM mim;
	mim.client = client;
	mim.mcollections = mcollections;
	mim.mcollectionsNum = InstrumentNum;
	mim.InstrumentIDs = InstrumentIDs;
	mim.md = md;
	mim.td = td;
	mim.running = &running;

	pthread_t insert_dmdmsg;
	pthread_create(&insert_dmdmsg, NULL, DMDMSG_insertIntoMongoDB, &mim);
	pthread_t revise_instrment;
	pthread_create(&revise_instrment, NULL, INSTRMENT_revise, &mim);
	pthread_t wait_event;
	pthread_create(&wait_event, NULL, EVENT_500ms_dmdmsg, &mim);

	TD_init(td);
	MD_init(md);

	//fflush(stdout);
	//_exit(0);
	//TD_reqQryInstrumentMarginRate(td, "IF1410");
	//sleep(2);
	//TD_reqQrySettlementInfo(td);
	//sleep(2);
	//TD_reqQryInstrument(td);

	//sleep(20);
	//running = 0;

	pthread_join(insert_dmdmsg, NULL);
	pthread_join(revise_instrment, NULL);
	pthread_join(wait_event, NULL);

	free(mdlogfilepath);
	free(tdlogfilepath);
	free(mdserver);
	free(tdserver);
	free(BrokerID);
	free(UserID);
	free(pd);
	free(mongodb_url_port);
	for (i = 0; i < InstrumentNum; ++i) {
		free(InstrumentIDs[i]);
	}
	
	MongoAPI_unglue_collections(mcollections, InstrumentNum);
	MongoAPI_destory_client(client);

	MD_free(md);
	MD_free(td);
	_exit(0);
}
