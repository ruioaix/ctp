#include "ctpcapi.h"
 
#include "io.h"
#include "mongoapi.h"
#include "threadfunc.h"
#include "verbose.h"

#include <stdio.h>
#include <pthread.h>
#include <mongoc.h>

int main(int argc, char **argv) {
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

	int running = 1;
	struct MongoIM mim;
	mim.client = client;
	mim.mcollections = mcollections;
	mim.mcollectionsNum = InstrumentNum;
	mim.InstrumentIDs = InstrumentIDs;
	mim.md = md;
	mim.running = &running;

	pthread_t insert_dmdmsg;
	pthread_create(&insert_dmdmsg, NULL, ProcessDMD, &mim);
	pthread_t revise_instrment;
	pthread_create(&revise_instrment, NULL, ProcessINS, &mim);

	MD_init(md);
	TD_init(td);

	//sleep(5);
	//TD_reqQryInstrumentMarginRate(td, "IF1410");
	//sleep(2);
	//TD_reqQrySettlementInfo(td);
	//sleep(2);
	//TD_reqQryInstrument(td);

	//sleep(20);
	//running = 0;

	pthread_join(insert_dmdmsg, NULL);
	pthread_join(revise_instrment, NULL);

	free(mdlogfilepath);
	free(tdlogfilepath);
	free(mdserver);
	free(tdserver);
	free(BrokerID);
	free(UserID);
	free(pd);
	free(mongodb_url_port);
	int i;
	for (i = 0; i < InstrumentNum; ++i) {
		free(InstrumentIDs[i]);
	}
	
	MongoAPI_unglue_collections(mcollections, InstrumentNum);
	MongoAPI_destory_client(client);

	//MD_free(md);
	sleep(2);
	_exit(0);
}
