#include "mduserapi.h"

#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>

#include "base.h"

#define delimiter ",;"

using namespace std;

/***create and delete************************************************************************************/
//create
CMdUserApi::CMdUserApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, char ** InstrumentIDs, int InstrumentNum)
{
	printlb("create both spi and api object - md.");
	m_nRequestID = 0;
	printlc("save m_nRequestID: %d", m_nRequestID);
	m_logFilePath = flowpath;
	printlc("save m_logFilePath: %s", m_logFilePath);
	m_server = servername;
	printlc("save m_server: %s", m_server);
	m_BrokerId = brokerid;
	printlc("save m_BrokerId: %s", m_BrokerId);
	m_InvestorId = inverstorid;
	printlc("save m_InvestorId: %s", m_InvestorId);
	m_Password = password;
	printlc("save m_Password: %s", m_Password);

	m_InstrumentIDs = InstrumentIDs;
	m_InstrumentNum = InstrumentNum;
	int i;
	for (i = 0; i < m_InstrumentNum; ++i) {
		printlc("save Instrument %d: %s", i, m_InstrumentIDs[i]);
	}

	mkdir(m_logFilePath, 0777);
	api = CThostFtdcMdApi::CreateFtdcMdApi(m_logFilePath);
	if (api == NULL) {
		isError("mdapi created failed");
	}
	api->RegisterSpi(this);
	api->RegisterFront(m_server);

	//init 12 callback point.
	m_fnOnFrontConnected = NULL;
	m_fnOnFrontDisconnected = NULL;
	m_fnOnHeartBeatWarning = NULL;
	m_fnOnRspUserLogin = NULL;
	m_fnOnRspUserLogout = NULL;
	m_fnOnRspError = NULL;
	m_fnOnRspSubMarketData = NULL;
	m_fnOnRspUnSubMarketData = NULL;
	m_fnOnRspSubForQuoteRsp = NULL;
	m_fnOnRspUnSubForQuoteRsp = NULL;
	m_fnOnRtnDepthMarketData = NULL;
	m_fnOnRtnForQuoteRsp = NULL;

	m_queue_size = 8192;
	printlc("prepare msg queue, size: %d", m_queue_size);
	m_queue = (CThostFtdcDepthMarketDataField *)smalloc(m_queue_size * sizeof(CThostFtdcDepthMarketDataField));
	if (m_queue == NULL) {
		exit(-1);
	}
	m_intime_second = (long *)smalloc(m_queue_size * sizeof(long));
	if (m_intime_second == NULL) {
		exit(-1);
	}
	m_intime_usecond = (long *)smalloc(m_queue_size * sizeof(long));
	if (m_intime_usecond == NULL) {
		exit(-1);
	}
	m_current_size = (int *)smalloc(m_queue_size * sizeof(int));
	if (m_current_size == NULL) {
		exit(-1);
	}
	for (i = 0; i < m_queue_size; ++i) {
		m_intime_second[i] = -1;
		m_intime_usecond[i] = -1;
	}
	m_header_index = 0;
	m_tail_index = 0;
	m_validmsg_size = 0;
}

//delete
CMdUserApi::~CMdUserApi(void)
{
	api->RegisterSpi(NULL);
	api->Release();
	api = NULL;
	free(m_queue);
	free(m_intime_second);
	free(m_intime_usecond);
	free(m_current_size);
	printlb("delete md");
}

/***13 functions, merge api functions in MdApi class to MdSpi class****************************************************/
//connect: include CreateFtdcMdApi, RegisterSpi, RegisterFront, Init.
void CMdUserApi::Init()
{
	printlb("api init");
	api->Init();
}

void CMdUserApi::Join()
{
	printlb("api join");
	api->Join();
}

const char *CMdUserApi::GetTradingDay()
{
	printlb("api gettradingday");
	return api->GetTradingDay();
}

void CMdUserApi::RegisterFront(char *pszFrontAddress) {
	printlb("api register front: %s", pszFrontAddress);
	api->RegisterFront(pszFrontAddress);
}

void CMdUserApi::RegisterNameServer(char *pszNsAddress) {
	printlb("api register name server: %s", pszNsAddress);
	api->RegisterNameServer(pszNsAddress);
}

void CMdUserApi::RegisterFensUserInfo(CThostFtdcFensUserInfoField *pFensUserInfo) {
	printlb("api register fens user info");
	printlc("pFensUserInfo->BrokerID: %s", pFensUserInfo->BrokerID);
	printlc("pFensUserInfo->UserID: %s", pFensUserInfo->UserID);
	printlc("pFensUserInfo->LoginMode: %d", pFensUserInfo->LoginMode);
	api->RegisterFensUserInfo(pFensUserInfo);
}

void CMdUserApi::RegisterSpi() {
	printlb("api register spi");
	api->RegisterSpi(this);
}

int CMdUserApi::SubscribeMarketData(char *ppInstrumentID[], int nCount) {
	printlb("api sub market data");
	int i;
	for (i = 0; i < nCount; ++i) {
		printlc("ppInstrumentID[%d]: %s", i, ppInstrumentID[i]);
	}
	return api->SubscribeMarketData(ppInstrumentID, nCount);
}

int CMdUserApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount) {
	printlb("api un sub market data");
	int i;
	for (i = 0; i < nCount; ++i) {
		printlc("ppInstrumentID[%d]: %s", i, ppInstrumentID[i]);
	}
	return api->UnSubscribeMarketData(ppInstrumentID, nCount);
}

int CMdUserApi::SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {
	printlb("api sub quota rsp");
	int i;
	for (i = 0; i < nCount; ++i) {
		printlc("ppInstrumentID[%d]: %s", i, ppInstrumentID[i]);
	}
	return api->SubscribeForQuoteRsp(ppInstrumentID, nCount);
}

int CMdUserApi::UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {
	printlb("api un sub quota rsp");
	int i;
	for (i = 0; i < nCount; ++i) {
		printlc("ppInstrumentID[%d]: %s", i, ppInstrumentID[i]);
	}
	return api->UnSubscribeForQuoteRsp(ppInstrumentID, nCount);
}

int CMdUserApi::ReqUserLogin()
{
	printlb("api req user login");

	CThostFtdcReqUserLoginField request = {0};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));
	printlc("request.BrokerID: %s", request.BrokerID);
	printlc("request.UserID: %s", request.UserID);
	printlc("request.Password: %s", request.Password);

	return api->ReqUserLogin(&request,++m_nRequestID);
}

int CMdUserApi::ReqUserLogout() {
	printlb("api req user logout");

	CThostFtdcUserLogoutField request = {0};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	printlc("request.BrokerID: %s", request.BrokerID);
	printlc("request.UserID: %s", request.UserID);

	return api->ReqUserLogout(&request,++m_nRequestID);
}

/***12 callback functions********************************************************************************/
//callback when connect successful.
//connect successful, and ReqUserLogin.
void CMdUserApi::OnFrontConnected()
{
	printlb("connected successfully.");
	if (m_fnOnFrontConnected != NULL) {
		(*m_fnOnFrontConnected)(this);
	}
	//login automatically after connected.
	printlb("request user login directly from here");
	ReqUserLogin();
}

//callback when connect unsuccessful.
void CMdUserApi::OnFrontDisconnected(int nReason)
{
	printlb("connected failed.");
	if (m_fnOnFrontDisconnected != NULL) {
		(*m_fnOnFrontDisconnected)(this, nReason);
	}
}

void CMdUserApi::OnHeartBeatWarning(int nTimeLapse) {
	printlb("OnHeartBeatWarning called, nTimeLapse: %d", nTimeLapse);
	if (m_fnOnHeartBeatWarning != NULL) {
		(*m_fnOnHeartBeatWarning)(this, nTimeLapse);
	}
}

//callback for ReqUserLogin.
void CMdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("OnRspUserLogin called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pRspUserLogin != NULL) {
		printlc("pRspUserLogin->TradingDay: %s", pRspUserLogin->TradingDay);
		printlc("pRspUserLogin->LoginTime: %s", pRspUserLogin->LoginTime);
		printlc("pRspUserLogin->BrokerID: %s", pRspUserLogin->BrokerID);
		printlc("pRspUserLogin->UserID: %s", pRspUserLogin->UserID); 
		printlc("pRspUserLogin->SystemName: %s", pRspUserLogin->SystemName);
		printlc("pRspUserLogin->FrontID: %d", pRspUserLogin->FrontID);
		printlc("pRspUserLogin->SessionID: %d", pRspUserLogin->SessionID);
		printlc("pRspUserLogin->MaxOrderRef: %s", pRspUserLogin->MaxOrderRef);
		printlc("pRspUserLogin->SHFETime: %s", pRspUserLogin->SHFETime); 
		printlc("pRspUserLogin->DCETime: %s", pRspUserLogin->DCETime);
		printlc("pRspUserLogin->CZCETime: %s", pRspUserLogin->CZCETime);
		printlc("pRspUserLogin->FFEXTime: %s", pRspUserLogin->FFEXTime);
		printlc("pRspUserLogin->INETime: %s", pRspUserLogin->INETime);
	}
	else {
		printlc("pRspUserLogin is NULL");
	}
	if (m_fnOnRspUserLogin != NULL) {
		(*m_fnOnRspUserLogin)(this, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
	}
	if ((pRspInfo==NULL || pRspInfo->ErrorID == 0) && pRspUserLogin) {
		printlb("SubscribeMarketData from here");
		api->SubscribeMarketData(m_InstrumentIDs, m_InstrumentNum);
		//printlb("SubscribeForQuoteRsp from here");
		//api->SubscribeForQuoteRsp(m_InstrumentIDs, m_InstrumentNum);
	}
}

void CMdUserApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printlb("OnRspUserLogout called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pUserLogout != NULL) {
		printlc("pUserLogout->BrokerID: %s, pUserLogout->UserID: %s", pUserLogout->BrokerID, pUserLogout->UserID);
	}
	else {
		printlc("pUserLogout is NULL");
	}
	if (m_fnOnRspUserLogout != NULL) {
		(*m_fnOnRspUserLogout)(this, pUserLogout, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("OnRspError called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %d, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (m_fnOnRspError != NULL) {
		(*m_fnOnRspError)(this, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("OnRspSubMarketData called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pSpecificInstrument != NULL) {
		printlc("pSpecificInstrument->InstrumentID: %s", pSpecificInstrument->InstrumentID);
	}
	else {
		printlc("pSpecificInstrument is NULL");
	}
	if (m_fnOnRspSubMarketData != NULL) {
		(*m_fnOnRspSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("OnRspUnSubMarketData called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pSpecificInstrument != NULL) {
		printlc("pSpecificInstrument->InstrumentID: %s", pSpecificInstrument->InstrumentID);
	}
	else {
		printlc("pSpecificInstrument is NULL");
	}
	if (m_fnOnRspUnSubMarketData != NULL) {
		(*m_fnOnRspUnSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	printlb("OnRspSubForQuoteRsp called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pSpecificInstrument != NULL) {
		printlc("pSpecificInstrument->InstrumentID: %s", pSpecificInstrument->InstrumentID);
	}
	else {
		printlc("pSpecificInstrument is NULL");
	}
	if (m_fnOnRspSubForQuoteRsp != NULL) {
		(*m_fnOnRspSubForQuoteRsp)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("OnRspUnSubMarketData called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pSpecificInstrument != NULL) {
		printlc("pSpecificInstrument->InstrumentID: %s", pSpecificInstrument->InstrumentID);
	}
	else {
		printlc("pSpecificInstrument is NULL");
	}
	if (m_fnOnRspUnSubForQuoteRsp) {
		(*m_fnOnRspUnSubForQuoteRsp)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	printlb("DepthMarketData comming");
	printld("pDepthMarketData->TradingDay: %s", pDepthMarketData->TradingDay);
	printld("pDepthMarketData->InstrumentID: %s", pDepthMarketData->InstrumentID);
	printld("pDepthMarketData->ExchangeID: %s", pDepthMarketData->ExchangeID);
	printld("pDepthMarketData->LastPrice: %f", pDepthMarketData->LastPrice);
	printld("pDepthMarketData->PreSettlementPrice: %f", pDepthMarketData->PreSettlementPrice);
	printld("pDepthMarketData->PreClosePrice: %f", pDepthMarketData->PreClosePrice);
	printld("pDepthMarketData->PreOpenInterest: %f", pDepthMarketData->PreOpenInterest);
	printld("pDepthMarketData->OpenPrice: %f", pDepthMarketData->OpenPrice);
	printld("pDepthMarketData->HighestPrice: %f", pDepthMarketData->HighestPrice);
	printld("pDepthMarketData->LowestPrice: %f", pDepthMarketData->LowestPrice);
	printld("pDepthMarketData->Volume: %d", pDepthMarketData->Volume);
	printld("pDepthMarketData->Turnover: %f", pDepthMarketData->Turnover);
	printld("pDepthMarketData->OpenInterest: %f", pDepthMarketData->OpenInterest);
	printld("pDepthMarketData->ClosePrice: %f", pDepthMarketData->ClosePrice);
	printld("pDepthMarketData->SettlementPrice: %f", pDepthMarketData->SettlementPrice);
	printld("pDepthMarketData->UpperLimitPrice: %f", pDepthMarketData->UpperLimitPrice);
	printld("pDepthMarketData->LowerLimitPrice: %f", pDepthMarketData->LowerLimitPrice);
	printld("pDepthMarketData->PreDelta: %f", pDepthMarketData->PreDelta);
	printld("pDepthMarketData->CurrDelta: %f", pDepthMarketData->CurrDelta);
	printld("pDepthMarketData->UpdateTime: %s", pDepthMarketData->UpdateTime);
	printld("pDepthMarketData->UpdateMillisec: %d", pDepthMarketData->UpdateMillisec);
	printld("pDepthMarketData->BidPrice1: %f", pDepthMarketData->BidPrice1);
	printld("pDepthMarketData->BidVolume1: %d", pDepthMarketData->BidVolume1);
	printld("pDepthMarketData->AskPrice1: %f", pDepthMarketData->AskPrice1);
	printld("pDepthMarketData->AskVolume1: %d", pDepthMarketData->AskVolume1);
	printld("pDepthMarketData->BidPrice2: %f", pDepthMarketData->BidPrice2);
	printld("pDepthMarketData->BidVolume2: %d", pDepthMarketData->BidVolume2);
	printld("pDepthMarketData->AskPrice2: %f", pDepthMarketData->AskPrice2);
	printld("pDepthMarketData->AskVolume2: %d", pDepthMarketData->AskVolume2);
	printld("pDepthMarketData->BidPrice3: %f", pDepthMarketData->BidPrice3);
	printld("pDepthMarketData->BidVolume3: %d", pDepthMarketData->BidVolume3);
	printld("pDepthMarketData->AskPrice3: %f", pDepthMarketData->AskPrice3);
	printld("pDepthMarketData->AskVolume3: %d", pDepthMarketData->AskVolume3);
	printld("pDepthMarketData->BidPrice4: %f", pDepthMarketData->BidPrice4);
	printld("pDepthMarketData->BidVolume4: %d", pDepthMarketData->BidVolume4);
	printld("pDepthMarketData->AskPrice4: %f", pDepthMarketData->AskPrice4);
	printld("pDepthMarketData->AskVolume4: %d", pDepthMarketData->AskVolume4);
	printld("pDepthMarketData->BidPrice5: %f", pDepthMarketData->BidPrice5);
	printld("pDepthMarketData->BidVolume5: %d", pDepthMarketData->BidVolume5);
	printld("pDepthMarketData->AskPrice5: %f", pDepthMarketData->AskPrice5);
	printld("pDepthMarketData->AskVolume5: %d", pDepthMarketData->AskVolume5);
	printld("pDepthMarketData->AveragePrice: %f", pDepthMarketData->AveragePrice);
	printld("pDepthMarketData->ActionDay: %s", pDepthMarketData->ActionDay);
	if (m_fnOnRtnDepthMarketData != NULL) {
		(*m_fnOnRtnDepthMarketData)(this, pDepthMarketData);
	}
	input_DMDQ(pDepthMarketData);
}

void CMdUserApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	printlb("OnRtnForQuoteRsp called.");
	if (pForQuoteRsp != NULL) {
		printlc("pForQuoteRsp->TradingDay: %s, pForQuoteRsp->InstrumentID: %s, pForQuoteRsp->ForQuoteSysID: %s, pForQuoteRsp->ForQuoteTime: %s, pForQuoteRsp->ActionDay: %s", \
				pForQuoteRsp->TradingDay, pForQuoteRsp->InstrumentID, pForQuoteRsp->ForQuoteSysID, pForQuoteRsp->ForQuoteTime, pForQuoteRsp->ActionDay);
	}
	else {
		printlc("pForQuoteRsp is NULL");
	}
	if (m_fnOnRtnForQuoteRsp != NULL) {
		(*m_fnOnRtnForQuoteRsp)(this, pForQuoteRsp);
	}
}

/***help functions**********************************************************************************/
void CMdUserApi::input_DMDQ(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	struct timeval tv;
	gettimeofday (&tv, NULL);
	long ts = tv.tv_sec;
    long tus = tv.tv_usec;
	m_queue[m_tail_index]=*pDepthMarketData;
	m_intime_second[m_tail_index] = ts;
	m_intime_usecond[m_tail_index] = tus;
	m_tail_index = (m_tail_index+1)&(m_queue_size-1);
	m_current_size[m_tail_index] = __sync_add_and_fetch(&m_validmsg_size, 1);
}

CThostFtdcDepthMarketDataField *CMdUserApi::output_DMDQ(long *ts, long *tus, int *size) {
	if (m_validmsg_size> 0) {
		__sync_fetch_and_sub(&m_validmsg_size, 1);
		*ts = m_intime_second[m_header_index];
		*tus = m_intime_usecond[m_header_index];
		*size = m_current_size[m_header_index];
		CThostFtdcDepthMarketDataField *h = m_queue + m_header_index;
		m_header_index = (m_header_index + 1)&(m_queue_size-1);
		return h;
	}
	return NULL;
}
