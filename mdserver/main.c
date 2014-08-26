#include "ctpcapi.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

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

void OnRtnDepthMarketData_i(void* md, CThostFtdcDepthMarketDataField *pDepthMarketData) {
	//CThostFtdcDepthMarketDataField *pDepthMarketData = MD_getOneDMDmsg(md);
	long ms;
	time_t s;
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	s = spec.tv_sec;
	ms = round(spec.tv_nsec/1E6);
	printf("InstrumentID: %s, LastPrice: %f, UpdateTime: %s, UpdateMillisec: %5d, realtime: %"PRIdMAX".%03ld\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, (intmax_t)s, ms);fflush(stdout);
	//printf(" TradingDay: %s\n InstrumentID: %s\n ExchangeID: %s\n ExchangeInstID: %s\n LastPrice: %f\n PreSettlementPrice: %f\n PreClosePrice: %f\n PreOpenInterest: %f\n OpenPrice: %f\n HighestPrice: %f\n LowestPrice: %f\n Volume: %d\n Turnover: %f\n OpenInterest: %f\n ClosePrice: %f\n SettlementPrice: %f\n UpperLimitPrice: %f\n LowerLimitPrice: %f\n PreDelta: %f\n CurrDelta: %f\n UpdateTime: %s\n UpdateMillisec: %d\n BidPrice1: %f\n BidVolume1: %d\n AskPrice1: %f\n AskVolume1: %d\n BidPrice2: %f\n BidVolume2: %d\n AskPrice2: %f\n AskVolume2: %d\n BidPrice3: %f\n BidVolume3: %d\n AskPrice3: %f\n AskVolume3: %d\n BidPrice4: %f\n BidVolume4: %d\n AskPrice4: %f\n AskVolume4: %d\n BidPrice5: %f\n BidVolume5: %d\n AskPrice5: %f\n AskVolume5: %d\n AveragePrice: %f\n ActionDay: %s\n", pDepthMarketData->TradingDay, pDepthMarketData->InstrumentID, pDepthMarketData->ExchangeID, pDepthMarketData->ExchangeInstID, pDepthMarketData->LastPrice, pDepthMarketData->PreSettlementPrice, pDepthMarketData->PreClosePrice, pDepthMarketData->PreOpenInterest, pDepthMarketData->OpenPrice, pDepthMarketData->HighestPrice, pDepthMarketData->LowestPrice, pDepthMarketData->Volume, pDepthMarketData->Turnover, pDepthMarketData->OpenInterest, pDepthMarketData->ClosePrice, pDepthMarketData->SettlementPrice, pDepthMarketData->UpperLimitPrice, pDepthMarketData->LowerLimitPrice, pDepthMarketData->PreDelta, pDepthMarketData->CurrDelta, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, pDepthMarketData->BidPrice1, pDepthMarketData->BidVolume1, pDepthMarketData->AskPrice1, pDepthMarketData->AskVolume1, pDepthMarketData->BidPrice2, pDepthMarketData->BidVolume2, pDepthMarketData->AskPrice2, pDepthMarketData->AskVolume2, pDepthMarketData->BidPrice3, pDepthMarketData->BidVolume3, pDepthMarketData->AskPrice3, pDepthMarketData->AskVolume3, pDepthMarketData->BidPrice4, pDepthMarketData->BidVolume4, pDepthMarketData->AskPrice4, pDepthMarketData->AskVolume4, pDepthMarketData->BidPrice5, pDepthMarketData->BidVolume5, pDepthMarketData->AskPrice5, pDepthMarketData->AskVolume5, pDepthMarketData->AveragePrice, pDepthMarketData->ActionDay);           
	printf("/********************************************************************************************************/\n");

}

void OnRtnForQuoteRsp_i(void* md, CThostFtdcForQuoteRspField *pForQuoteRsp) {
	//printf("InstrumentID: %s, ForQuoteSysID: %s\n", pForQuoteRsp->InstrumentID, pForQuoteRsp->ForQuoteSysID);fflush(stdout);
}

int main(int argc, char **argv) {
	//void *md = MD_create("/tmp/md", "tcp://222.66.97.241:41213", "9016766", "9016766", "1111111");
	char *i1[2] = {"IF1409", "IF1410"};
	void *md = MD_create("/tmp/md", "tcp://27.17.62.149:40213", "1035", "00000008", "123456", i1, 2);
	MD_RegOnFrontConnected(md, OnFrontConnected_i);
	MD_RegOnFrontDisconnected(md, OnFrontDisconnected_i);
	MD_RegOnRspUserLogin(md, OnRspUserLogin_i);
	MD_RegOnRtnDepthMarketData(md, OnRtnDepthMarketData_i);
	MD_RegOnRtnForQuoteRsp(md, OnRtnForQuoteRsp_i);

	MD_init(md);
	sleep(9);
	MD_free(md);
	return 0;
}
