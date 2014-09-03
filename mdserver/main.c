#include "ctpcapi.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>
#include <pthread.h>

#include <bson.h>
#include <mongoc.h>

struct MongoIM {
	void *md;
	mongoc_client_t *client;
	mongoc_collection_t *collection;
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
	mongoc_collection_t *collection = mim->collection;
	while (1) {
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
			insert_mongodb(client, collection, pDepthMarketData, 0.1, 0.1);
		}
	}
}

int main(int argc, char **argv) {
	//void *md = MD_create("/tmp/md", "tcp://222.66.97.241:41213", "9016766", "9016766", "1111111");
	char *i1[2] = {"IF1410"};
	void *md = MD_create("/tmp/md", "tcp://27.17.62.149:40213", "1035", "00000008", "123456", i1, 1);


	mongoc_init ();
	mongoc_client_t *client = mongoc_client_new ("mongodb://ctp_md:ctp_md@localhost:27017/?authSource=ctp");
	mongoc_collection_t *collection = mongoc_client_get_collection (client, "ctp", "ctp");

	struct MongoIM mim;
	mim.client = client;
	mim.collection = collection;
	mim.md = md;

	pthread_t p;
	pthread_create(&p, NULL, ProcessDMD, &mim);


	MD_init(md);


	pthread_join(p, NULL);

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);


	MD_free(md);
	return 0;
}
