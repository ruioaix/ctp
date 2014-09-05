#include "ctpcapi.h"
#include "mdcallback.h"
#include <stdio.h>

void OnFrontConnected_d(void* md) {
	printf("OnFrontConnected called, connected successfully.\n");
	fflush(stdout);
}

void OnFrontDisconnected_d(void* md, int nReason) {
	printf("OnFrontDisconnected called, connected failed.\n\t ");
	switch(nReason)
	{
		case 0x1001:
			printf("0x1001 network read failed.\n");
			break;
		case 0x1002:
			printf("0x1002 network write failed.\n");
			break;
		case 0x2001:
			printf("0x2001 receive heartbeat timeout.\n");
			break;
		case 0x2002:
			printf("0x2002 send heartbeat failed.\n");
			break;
		case 0x2003:
			printf("0x2003 receive error msg.\n");
			break;
		default:
			printf("%x unkown error", nReason);
			break;
	}
	fflush(stdout);
}

void OnHeartBeatWarning_d(void* md, int nTimeLapse) {
	printf("OnHeartBeatWarning called, nTimeLapse: %d\n", nTimeLapse);
	fflush(stdout);
}

static void print_CThostFtdcRspUserLoginField(CThostFtdcRspUserLoginField *pRspUserLogin) {
	printf("pRspUserLogin->TradingDay: %s, pRspUserLogin->LoginTime: %s, pRspUserLogin->BrokerID: %s, pRspUserLogin->UserID: %s, pRspUserLogin->SystemName: %s, \
			pRspUserLogin->FrontID: %d, pRspUserLogin->SessionID: %d, pRspUserLogin->MaxOrderRef: %s, pRspUserLogin->SHFETime: %s, pRspUserLogin->DCETime: %s, \
			pRspUserLogin->CZCETime: %s, pRspUserLogin->FFEXTime: %s, pRspUserLogin->INETime: %s\n",\
		pRspUserLogin->TradingDay, pRspUserLogin->LoginTime, pRspUserLogin->BrokerID, pRspUserLogin->UserID, pRspUserLogin->SystemName,\
		pRspUserLogin->FrontID, pRspUserLogin->SessionID, pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime, pRspUserLogin->DCETime, \
		pRspUserLogin->CZCETime, pRspUserLogin->FFEXTime, pRspUserLogin->INETime);
	fflush(stdout);
}

void OnRspUserLogin_d(void *md, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspUserLogin called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
	if (pRspUserLogin != NULL) {
		printf("\t ");
		print_CThostFtdcRspUserLoginField(pRspUserLogin);
	}
	else {
		printf("\t pRspUserLogin is NULL\n");
	}
	fflush(stdout);
}

void OnRspUserLogout_d(void *md, CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspUserLogout called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
	if (pUserLogout != NULL) {
		printf("\t pUserLogout->BrokerID: %s, pUserLogout->UserID: %s\n", pUserLogout->BrokerID, pUserLogout->UserID);
	}
	else {
		printf("\t pUserLogout is NULL\n");
	}
}

void OnRspError_d(void* md, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspError called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
}

void OnRspSubMarketData_d(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspSubMarketData called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
	if (pSpecificInstrument != NULL) {
		printf("\t pSpecificInstrument->InstrumentID: %s\n", pSpecificInstrument->InstrumentID);
	}
	else {
		printf("\t pSpecificInstrument is NULL\n");
	}

}

void OnRspUnSubMarketData_d(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspUnSubMarketData called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
	if (pSpecificInstrument != NULL) {
		printf("\t pSpecificInstrument->InstrumentID: %s\n", pSpecificInstrument->InstrumentID);
	}
	else {
		printf("\t pSpecificInstrument is NULL\n");
	}
}

void OnRspSubForQuoteRsp_d(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspSubForQuoteRsp called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
	if (pSpecificInstrument != NULL) {
		printf("\t pSpecificInstrument->InstrumentID: %s\n", pSpecificInstrument->InstrumentID);
	}
	else {
		printf("\t pSpecificInstrument is NULL\n");
	}
}

void OnRspUnSubForQuoteRsp_d(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast) {
	printf("OnRspUnSubMarketData called.\n");
	printf("\t nRequestId: %d, bIsLast: %d\n", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printf("\t pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printf("\t pRspInfo is NULL\n");
	}
	if (pSpecificInstrument != NULL) {
		printf("\t pSpecificInstrument->InstrumentID: %s\n", pSpecificInstrument->InstrumentID);
	}
	else {
		printf("\t pSpecificInstrument is NULL\n");
	}
}

void OnRtnDepthMarketData_d(void* md, CThostFtdcDepthMarketDataField *pDepthMarketData) {
	//printf("OnRtnDepthMarketData called.\n");
}

void OnRtnForQuoteRsp_d(void* md, CThostFtdcForQuoteRspField *pForQuoteRsp) {
	printf("OnRtnForQuoteRsp called.\n");
	if (pForQuoteRsp != NULL) {
		printf("\t pForQuoteRsp->TradingDay: %s, pForQuoteRsp->InstrumentID: %s, pForQuoteRsp->ForQuoteSysID: %s, pForQuoteRsp->ForQuoteTime: %s, pForQuoteRsp->ActionDay: %s\n", \
				pForQuoteRsp->TradingDay, pForQuoteRsp->InstrumentID, pForQuoteRsp->ForQuoteSysID, pForQuoteRsp->ForQuoteTime, pForQuoteRsp->ActionDay);
	}
	else {
		printf("\t pForQuoteRsp is NULL\n");
	}
}
