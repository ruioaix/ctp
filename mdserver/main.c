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
	long ms;
	time_t s;
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	s = spec.tv_sec;
	ms = round(spec.tv_nsec/1E6);
	printf("InstrumentID: %s, LastPrice: %f, UpdateTime: %s, UpdateMillisec: %5d, realtime: %"PRIdMAX".%03ld\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, (intmax_t)s, ms);fflush(stdout);
}

void OnRtnForQuoteRsp_i(void* md, CThostFtdcForQuoteRspField *pForQuoteRsp) {
	printf("InstrumentID: %s, ForQuoteSysID: %s\n", pForQuoteRsp->InstrumentID, pForQuoteRsp->ForQuoteSysID);fflush(stdout);
}

int main(int argc, char **argv) {
	void *md = Create_MD();
	MD_RegOnFrontConnected(md, OnFrontConnected_i);
	MD_RegOnFrontDisconnected(md, OnFrontDisconnected_i);
	MD_RegOnRspUserLogin(md, OnRspUserLogin_i);
	MD_RegOnRtnDepthMarketData(md, OnRtnDepthMarketData_i);
	MD_RegOnRtnForQuoteRsp(md, OnRtnForQuoteRsp_i);
	MD_Connect(md, "/tmp/", "tcp://222.66.97.241:41213", "9016766", "9016766", "1111111");
	MD_Subscribe(md, "IF1409,IF1410", "IF1410");
	sleep(5);
	//MD_Unsubscribe(md, "IF1409", "IF1410");
	sleep(2);
	MD_Disconnect(md);
	sleep(2);
	MD_ReleaseMdApi(md);
	return 0;
}
