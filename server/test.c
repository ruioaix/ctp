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

	int i,j,k=0;
	//int ovolume[10000] = {0};
	int tvolume[10000] = {0};
	/*
	struct BAR *bar = create_1MTYPE_BAR_from_MongoDB(mcollections[0], 20140919, 20140922);
	sprintf(bar->InstrumentID, "%s", InstrumentIDs[0]);
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		for (j = 0; j < BARNUM_1MIN1DAY; ++j) {
			printf("k:%05d, nN: %d, bN: %d, InID: %s, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->InstrumentID, bar->bars[i]->YMD, bar->bars[i]->btimeHMS[j], bar->bars[i]->etimeHMS[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
			ovolume[i] += bar->bars[i]->volume[j];
		}
	}
	free_BAR(bar);

	int kk;
	for (kk = 2; kk <= 2; ++kk) {
		bar = create_BAR(kk, mcollections[0], 20140919, 20140922);
		sprintf(bar->InstrumentID, "%s", InstrumentIDs[0]);
		for (i = bar->head; i <= bar->tail; ++i) {
			if (bar->bars[i] == NULL) continue;
			tvolume[i] = 0;
			for (j = 0; j < bar->num; ++j) {
				printf("k:%05d, nN: %d, bN: %d, InID: %s, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->InstrumentID, bar->bars[i]->YMD, bar->bars[i]->btimeHMS[j], bar->bars[i]->etimeHMS[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
				tvolume[i] += bar->bars[i]->volume[j];
			}
			printf("%d---%d\n", tvolume[i], ovolume[i]);
			if (tvolume[i] != ovolume[i]) {
				isError("xx");
			}
		}
		free_BAR(bar);
	}

	bar = create_BAR(135, mcollections[0], 20140919, 20140922);
	sprintf(bar->InstrumentID, "%s", InstrumentIDs[0]);
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		tvolume[i] = 0;
		for (j = 0; j < bar->num; ++j) {
			printf("k:%05d, nN: %d, bN: %d, InID: %s, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->InstrumentID, bar->bars[i]->YMD, bar->bars[i]->btimeHMS[j], bar->bars[i]->etimeHMS[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
			tvolume[i] += bar->bars[i]->volume[j];
		}
		printf("%d---%d\n", tvolume[i], ovolume[i]);
		if (tvolume[i] != ovolume[i]) {
			isError("xx");
		}
	}
	free_BAR(bar);

	bar = create_BAR(273, mcollections[0], 20140919, 20140922);
	sprintf(bar->InstrumentID, "%s", InstrumentIDs[0]);
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		tvolume[i] = 0;
		for (j = 0; j < bar->num; ++j) {
			printf("k:%05d, nN: %d, bN: %d, InID: %s, YMD: %06d, b-etimeHMS: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->InstrumentID, bar->bars[i]->YMD, bar->bars[i]->btimeHMS[j], bar->bars[i]->etimeHMS[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
			tvolume[i] += bar->bars[i]->volume[j];
		}
		printf("%d---%d\n", tvolume[i], ovolume[i]);
		if (tvolume[i] != ovolume[i]) {
			isError("xx");
		}
	}
	free_BAR(bar);

	*/
	
	int barLen[12] = {1, 2, 3, 4, 5, 10, 15, 20, 40, 60, 135, 273};
	struct BAR **barA;
	create_Multi_BAR(12, barLen, mcollections[0], 20140919, 20140922, &barA);
	int kk;
	for (kk = 0; kk < 63; ++kk) {
		struct BAR *bar = barA[kk];
		if (bar == NULL) continue;
		for (i = bar->head; i <= bar->tail; ++i) {
			if (bar->bars[i] == NULL) continue;
			for (j = 0; j < bar->num; ++j) {
				printf("k:%05d, nN: %d, bN: %d, YMD: %06d, b-etimeHMSM: %06d-%06d, o-cPrice: %f-%f, u-lPrice: %f-%f, vol:%d\n", k++, bar->barLen, bar->num, bar->bars[i]->YMD, bar->bars[i]->btimeHMSM[j], bar->bars[i]->etimeHMSM[j], bar->bars[i]->openPrice[j], bar->bars[i]->closePrice[j], bar->bars[i]->uplimitPrice[j], bar->bars[i]->lowlimitPrice[j], bar->bars[i]->volume[j]);
				tvolume[i] += bar->bars[i]->volume[j];
			}
		}
		free_BAR(bar);
	}
	free(barA);


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
