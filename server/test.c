#include "ctpcapi.h"
 
#include "io.h"
#include "mongoapi.h"
#include "threadfunc.h"
#include "verbose.h"
#include "bar.h"

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

	mongoc_client_t *client = MongoAPI_create_client(mongodb_url_port);
	mongoc_collection_t **mcollections = MongoAPI_glue_collections(client, InstrumentIDs, InstrumentNum, BrokerID, UserID);

	int i;
	struct BAR *bar = create_1MTYPE_BAR_from_MongoDB(mcollections[0], 20140919, 20140922);
	sprintf(bar->InstrumentID, "%s", InstrumentIDs[0]);
	int k=0;
	int j;
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		for (j = 0; j < BARNUM_1MIN1DAY; ++j) {
			printf("k:%05d, nN: %d, bN: %d, InID: %s, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->InstrumentID, bar->bars[i]->YMD, bar->bars[i]->btimeHMS[j], bar->bars[i]->etimeHMS[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
		}
	}
	free_BAR(bar);
	bar = create_BAR(4, mcollections[0], 20140919, 20140922);
	sprintf(bar->InstrumentID, "%s", InstrumentIDs[0]);
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		for (j = 0; j < bar->num; ++j) {
			printf("k:%05d, nN: %d, bN: %d, InID: %s, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->InstrumentID, bar->bars[i]->YMD, bar->bars[i]->btimeHMS[j], bar->bars[i]->etimeHMS[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
		}
	}
	//free(bar);

	free_BAR(bar);
	free(mdlogfilepath);
	free(tdlogfilepath);
	free(mdserver);
	free(tdserver);
	free(BrokerID);
	free(InvestorID);
	free(UserID);
	free(pd);
	free(UserProductInfo);
	free(mongodb_url_port);
	for (i = 0; i < InstrumentNum; ++i) {
		free(InstrumentIDs[i]);
	}
	
	MongoAPI_unglue_collections(mcollections, InstrumentNum);
	MongoAPI_destory_client(client);

	_exit(0);
}
