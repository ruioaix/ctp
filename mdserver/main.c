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

void OnFrontConnected_i(void * md) {
	printf("connect successful.\n");
}

void OnFrontDisconnected_i(void *md, int nReason) {
	printf("connect unsuccessful.\n");
}

void OnRspUserLogin_i(void *md, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("Date: %s\n", pRspUserLogin->TradingDay);
	printf("LoginTime: %s\n", pRspUserLogin->LoginTime);
	printf("BrokerID: %s, UserID: %s\n", pRspUserLogin->BrokerID, pRspUserLogin->UserID);
	printf("SystemName: %s\n", pRspUserLogin->SystemName);
	printf("FrontID: %d, SessionID: %d\n", pRspUserLogin->FrontID, pRspUserLogin->SessionID);
	printf("MaxOrderRef: %s\n", pRspUserLogin->MaxOrderRef);
	printf("SHFETime: %s\n", pRspUserLogin->SHFETime);
	printf("DCETime: %s\n", pRspUserLogin->DCETime);
	printf("FFEXTime: %s\n", pRspUserLogin->FFEXTime);
	printf("INETime: %s\n", pRspUserLogin->INETime);
	printf("ErrorID: %d, ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
}

void OnRtnDepthMarketData_i(void* md, CThostFtdcDepthMarketDataField *pDepthMarketData_p) {

}

void OnRtnForQuoteRsp_i(void* md, CThostFtdcForQuoteRspField *pForQuoteRsp) {
	//printf("InstrumentID: %s, ForQuoteSysID: %s\n", pForQuoteRsp->InstrumentID, pForQuoteRsp->ForQuoteSysID);fflush(stdout);
}

void insert_mongodb(mongoc_client_t *client, mongoc_collection_t *collection, CThostFtdcDepthMarketDataField *pd) {

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
			"ActionDay", BCON_UTF8 (pd->ActionDay)
				);

	if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, NULL)) {
		printf("error insert db\n");
	}

	bson_destroy(doc);

}


void *ProcessDMD(void *mim_p) {
	struct MongoIM *mim = mim_p;
	void *md = mim->md;
	mongoc_client_t *client = mim->client;
	mongoc_collection_t *collection = mim->collection;
	while (1) {
		double arrivetime;
		CThostFtdcDepthMarketDataField *pDepthMarketData = MD_getOneDMDmsg(md, &arrivetime);
		struct timeval tv;
		gettimeofday (&tv, NULL);
		double processtime = tv.tv_sec + ((double)(tv.tv_usec))/1E6;
		printf("arrived time: %.6f\n", arrivetime);
		printf("process time: %.6f\n", processtime);
		printf("delayed time: %.6f\n", processtime-arrivetime);

		printf("/********************************************************************************************************/\n");
		insert_mongodb(client, collection, pDepthMarketData);
	}
}

int main(int argc, char **argv) {
	//void *md = MD_create("/tmp/md", "tcp://222.66.97.241:41213", "9016766", "9016766", "1111111");
	char *i1[2] = {"IF1409"};
	void *md = MD_create("/tmp/md", "tcp://27.17.62.149:40213", "1035", "00000008", "123456", i1, 1);
	MD_RegOnFrontConnected(md, OnFrontConnected_i);
	MD_RegOnFrontDisconnected(md, OnFrontDisconnected_i);
	MD_RegOnRspUserLogin(md, OnRspUserLogin_i);
	MD_RegOnRtnDepthMarketData(md, OnRtnDepthMarketData_i);
	MD_RegOnRtnForQuoteRsp(md, OnRtnForQuoteRsp_i);

	mongoc_client_t *client;
	mongoc_collection_t *collection;
	mongoc_init ();
	client = mongoc_client_new ("mongodb://localhost:27017/");
	collection = mongoc_client_get_collection (client, "ctp", "ctp");

	struct MongoIM mim;
	mim.client = client;
	mim.collection = collection;
	mim.md = md;

	pthread_t p;
	pthread_create(&p, NULL, ProcessDMD, &mim);

	MD_init(md);
	sleep(30);

	mongoc_collection_destroy (collection);
	mongoc_client_destroy (client);

	MD_free(md);
	return 0;
}
