#include "ctpcapi.h"
#include "vbmal.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>

#include <bson.h>
#include <mongoc.h>


void readinfo(char *filename, char **logfilepath, char **server, char **BrokerID, char **UserID, char **pd, char *(*InstrumentIDs)[], int *InstrumentNum) {
	FILE *fp = sfopen(filename, "r");
	char line[1000];
	int i = 0;
	int j = 0;
	while(fgets(line, 1000, fp)) {
		switch(i++) {
			case 0:
				*logfilepath = smalloc(strlen(line));
				memcpy(*logfilepath, line, strlen(line));
				(*logfilepath)[strlen(line)-1] = '\0';
				break;
			case 1:
				*server = smalloc(strlen(line));
				memcpy(*server, line, strlen(line));
				(*server)[strlen(line)-1] = '\0';
				break;
			case 2:
				*BrokerID= smalloc(strlen(line));
				memcpy(*BrokerID, line, strlen(line));
				(*BrokerID)[strlen(line)-1] = '\0';
				break;
			case 3:
				*UserID= smalloc(strlen(line));
				memcpy(*UserID, line, strlen(line));
				(*UserID)[strlen(line)-1] = '\0';
				break;
			case 4:
				*pd = smalloc(strlen(line));
				memcpy(*pd, line, strlen(line));
				(*pd)[strlen(line)-1] = '\0';
				break;
			default:
				(*InstrumentIDs)[j] = smalloc(strlen(line));
				memcpy((*InstrumentIDs)[j], line, strlen(line));
				(*InstrumentIDs)[j][strlen(line)-1] = '\0';
				j++;
				break;
		}
	}
	*InstrumentNum = j;
	fclose(fp);
}

struct MongoIM {
	void *md;
	mongoc_client_t *client;
	mongoc_collection_t **mcollections;
	int mcollectionsNum;
	char **InstrumentIDs;	
	int *running;
};

void insert_mongodb(mongoc_client_t *client, mongoc_collection_t *collection, CThostFtdcDepthMarketDataField *pd, double arrivedtime, double processtime) {

	bson_t *doc = BCON_NEW (
			"TradingDay", BCON_UTF8 (pd->TradingDay),
			"InstrumentID", BCON_UTF8 (pd->InstrumentID),
			"ExchangeID", BCON_UTF8 (pd->ExchangeID),
			"LastPrice", BCON_DOUBLE (pd->LastPrice),
			"PreSettlementPrice", BCON_DOUBLE (pd->PreSettlementPrice),
			"PreClosePrice", BCON_DOUBLE (pd->PreClosePrice),
			"PreOpenInterest", BCON_DOUBLE (pd->PreOpenInterest),
			"OpenPrice", BCON_DOUBLE (pd->OpenPrice),
			"HighestPrice", BCON_DOUBLE (pd->HighestPrice),
			"LowestPrice", BCON_DOUBLE (pd->LowestPrice),
			"Volume", BCON_INT32 (pd->Volume),
			"Turnover", BCON_DOUBLE (pd->Turnover),
			"OpenInterest", BCON_DOUBLE (pd->OpenInterest),
			"ClosePrice", BCON_DOUBLE (pd->ClosePrice),
			"SettlementPrice", BCON_DOUBLE (pd->SettlementPrice),
			"UpperLimitPrice", BCON_DOUBLE (pd->UpperLimitPrice),
			"LowerLimitPrice", BCON_DOUBLE (pd->LowerLimitPrice),
			"PreDelta", BCON_DOUBLE (pd->PreDelta),
			"CurrDelta", BCON_DOUBLE (pd->CurrDelta),
			"UpdateTime", BCON_UTF8 (pd->UpdateTime),
			"UpdateMillisec", BCON_INT32 (pd->UpdateMillisec),
			"BidPrice1", BCON_DOUBLE (pd->BidPrice1),
			"BidVolume1", BCON_INT32 (pd->BidVolume1),
			"AskPrice1", BCON_DOUBLE (pd->AskPrice1),
			"AskVolume1", BCON_INT32 (pd->AskVolume1),
			"BidPrice2", BCON_DOUBLE (pd->BidPrice2),
			"BidVolume2", BCON_INT32 (pd->BidVolume2),
			"AskPrice2", BCON_DOUBLE (pd->AskPrice2),
			"AskVolume2", BCON_INT32 (pd->AskVolume2),
			"BidPrice3", BCON_DOUBLE (pd->BidPrice3),
			"BidVolume3", BCON_INT32 (pd->BidVolume3),
			"AskPrice3", BCON_DOUBLE (pd->AskPrice3),
			"AskVolume3", BCON_INT32 (pd->AskVolume3),
			"BidPrice4", BCON_DOUBLE (pd->BidPrice4),
			"BidVolume4", BCON_INT32 (pd->BidVolume4),
			"AskPrice4", BCON_DOUBLE (pd->AskPrice4),
			"AskVolume4", BCON_INT32 (pd->AskVolume4),
			"BidPrice5", BCON_DOUBLE (pd->BidPrice5),
			"BidVolume5", BCON_INT32 (pd->BidVolume5),
			"AskPrice5", BCON_DOUBLE (pd->AskPrice5),
			"AskVolume5", BCON_INT32 (pd->AskVolume5),
			"AveragePrice", BCON_DOUBLE (pd->AveragePrice),
			"ActionDay", BCON_UTF8 (pd->ActionDay),
			"DMDMsgArrivedTime", BCON_DOUBLE (arrivedtime),
			"DMDMsgProcessTime", BCON_DOUBLE (processtime),
			"DMDMsgDelayedTime", BCON_DOUBLE (processtime - arrivedtime)
				);

	int mci = mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, NULL);
	while (!mci) {
		printf("mongo insert error.\n");
		sleep(1);
		mci = mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, NULL);
	}
	bson_destroy(doc);

}


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
			struct timeval tv;
			gettimeofday (&tv, NULL);
			//printf("arrived time: %ld.%06d, EasyRead: %s", ts, tus, ctime(&ts));
			//printf("process time: %ld.%06d, EasyRead: %s", tv.tv_sec, (int)tv.tv_usec, ctime(&(tv.tv_sec)));
			printf("updated time: %s, update mill time : %4d, arrive: %ld.%06ld, delay: %f, valid size: %3d\n", pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, ts, tus, tv.tv_sec-ts+(tv.tv_usec-tus)*1E-6, size);
			int i;
			for (i = 0; i < mcollectionsNum; ++i) {
				if (strcmp(pDepthMarketData->InstrumentID, InstrumentIDs[i]) == 0) {
					break;
				}
			}
			insert_mongodb(client, mcollections[i], pDepthMarketData, ts+tus*1E-6, tv.tv_sec+(tv.tv_usec)*1E-6);
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
	char *logfilepath, *server, *BrokerID, *UserID, *pd;
	readinfo(file, &logfilepath, &server, &BrokerID, &UserID, &pd, &InstrumentIDs, &InstrumentNum);
	void *md = MD_create(logfilepath, server, BrokerID, UserID, pd, InstrumentIDs, InstrumentNum);

	mongoc_init ();
	mongoc_client_t *client = mongoc_client_new ("mongodb://localhost:27017");
	mongoc_collection_t **mcollections = smalloc((InstrumentNum+1) * sizeof(mongoc_collection_t *));

	char mongo_collection_name[1000];
	int i;
	for (i = 0; i < InstrumentNum; ++i) {
		sprintf(mongo_collection_name, "%s_%s_%s", BrokerID, UserID, InstrumentIDs[i]);
		mcollections[i] = mongoc_client_get_collection (client, "ctp", mongo_collection_name);
	}
	sprintf(mongo_collection_name, "%s_%s_%s", BrokerID, UserID, "other");
	mcollections[i] = mongoc_client_get_collection(client, "ctp", mongo_collection_name);

	int running = 1;
	struct MongoIM mim;
	mim.client = client;
	mim.mcollections = mcollections;
	mim.mcollectionsNum = InstrumentNum;
	mim.InstrumentIDs = InstrumentIDs;
	mim.md = md;
	mim.running = &running;

	pthread_t p;
	pthread_create(&p, NULL, ProcessDMD, &mim);
	//pthread_t instrment;
	//pthread_create(&instrment, NULL, ProcessINS, &mim);


	MD_init(md);

	sleep(10);
	running = 0;


	pthread_join(p, NULL);


	free(logfilepath);
	free(server);
	free(BrokerID);
	free(UserID);
	free(pd);
	for (i = 0; i < InstrumentNum; ++i) {
		free(InstrumentIDs[i]);
		mongoc_collection_destroy (mcollections[i]);
	}
	mongoc_collection_destroy (mcollections[i]);
	free(mcollections);
	mongoc_client_destroy(client);

	MD_free(md);
	_exit(0);
}
