#include "threadfunc.h"

#include "ctpcapi.h"
#include "mongoapi.h"
#include "vbmal.h"
#include <string.h>

void *ProcessDMD(void *mim_p) {
	struct MongoIM *mim = mim_p;
	void *md = mim->md;
	mongoc_client_t *client = mim->client;
	mongoc_collection_t **mcollections = mim->mcollections;
	int mcollectionsNum = mim->mcollectionsNum;
	char **InstrumentIDs = mim->InstrumentIDs;
	while (*(mim->running)) {
		long ts;
		long tus;
		int size;
		CThostFtdcDepthMarketDataField *pDepthMarketData = MD_getOneDMDmsg(md, &ts, &tus, &size);
		if (pDepthMarketData != NULL) {

			struct timespec tv;
			clock_gettime(CLOCK_REALTIME, &tv);

			printf("updated time: %s, update mill time : %4d, arrive: %ld.%06ld, delay: %f, valid size: %3d\n", pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, ts, tus, tv.tv_sec-ts+(tv.tv_nsec-tus)*1E-6, size);
			int i;
			for (i = 0; i < mcollectionsNum; ++i) {
				if (strcmp(pDepthMarketData->InstrumentID, InstrumentIDs[i]) == 0) {
					break;
				}
			}
			insert_mongodb(client, mcollections[i], pDepthMarketData, ts+tus*1E-6, tv.tv_sec+(tv.tv_nsec)*1E-6);
		}
	}
	return NULL;
}

void *ProcessINS(void *mim_p) {
	struct MongoIM *mim = mim_p;
	void *md = mim->md;
	//mongoc_client_t *client = mim->client;
	//mongoc_collection_t **mcollections = mim->mcollections;
	int mcollectionsNum = mim->mcollectionsNum;
	char **InstrumentIDs = mim->InstrumentIDs;
	time_t t = time(NULL);
	struct tm *tm = gmtime(&t);
	int year_month = (tm->tm_year-100)*100 + tm->tm_mon + 2;
	while (1) {
		sleep(2000);
		time_t t = time(NULL);
		struct tm *tm = gmtime(&t);
		if (tm->tm_hour == 23) {
			int now_year_month = (tm->tm_year-100)*100 + tm->tm_mon + 2;
			printf("%d\n", now_year_month);
			if (now_year_month != year_month) {
				char insname_old[10];
				char insname_new[10];
				sprintf(insname_old, "IF%d", year_month);
				sprintf(insname_new, "IF%d", now_year_month+1);
				int i;
				for (i = 0; i < mcollectionsNum; ++i) {
					if (strcmp(insname_old, InstrumentIDs[i]) == 0) {
						free(InstrumentIDs[i]);
						InstrumentIDs[i] = smalloc(strlen(insname_new)+1);
						memcpy(InstrumentIDs[i], insname_new, strlen(insname_new));
						InstrumentIDs[i][strlen(insname_new)] = '\0';
					}
				}
				//sprintf(insname, "IF%d", now_year_month);
				MD_subscribeMarketData(md, InstrumentIDs, mcollectionsNum); 
				year_month = now_year_month;
			}
		}
	}
	return NULL;
}
