#include "mduserapi.h"

#include <cstring>
#include <ctime>

using namespace std;

/***create and delete************************************************************************************/
//create
CMdUserApi::CMdUserApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, char ** InstrumentIDs, int InstrumentNum)
{
	printmlb("create both spi and api object - md.");
	m_nRequestID = 0;
	printmlc("save m_nRequestID: %d", m_nRequestID);
	m_logFilePath = flowpath;
	printmlc("save m_logFilePath: %s", m_logFilePath);
	m_server = servername;
	printmlc("save m_server: %s", m_server);
	m_BrokerId = brokerid;
	printmlc("save m_BrokerId: %s", m_BrokerId);
	m_InvestorId = inverstorid;
	printmlc("save m_InvestorId: %s", m_InvestorId);
	m_Password = password;
	printmlc("save m_Password: %s", m_Password);

	m_InstrumentIDs = InstrumentIDs;
	m_InstrumentNum = InstrumentNum;
	int i;
	for (i = 0; i < m_InstrumentNum; ++i) {
		printmlc("save Instrument %d: %s", i, m_InstrumentIDs[i]);
	}

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
	m_fnOnRtnDepthMarketData = NULL;

	m_queue_size = 8192;
	printmlc("prepare msg queue, size: %d", m_queue_size);
	m_queue = (CThostFtdcDepthMarketDataField *)malloc(m_queue_size * sizeof(CThostFtdcDepthMarketDataField));
	if (m_queue == NULL) {
		exit(-1);
	}
	m_intime_second = (long *)malloc(m_queue_size * sizeof(long));
	if (m_intime_second == NULL) {
		exit(-1);
	}
	m_intime_usecond = (long *)malloc(m_queue_size * sizeof(long));
	if (m_intime_usecond == NULL) {
		exit(-1);
	}
	m_current_size = (int *)malloc(m_queue_size * sizeof(int));
	if (m_current_size == NULL) {
		exit(-1);
	}
	m_current_size[0]=0;
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
	printmlb("delete md");
}

/***11 functions, merge api functions in MdApi class to MdSpi class****************************************************/
//connect: include CreateFtdcMdApi, RegisterSpi, RegisterFront, Init.
void CMdUserApi::Init()
{
	printmlb("api init");
	api->Init();
}

void CMdUserApi::Join()
{
	printmlb("api join");
	api->Join();
}

const char *CMdUserApi::GetTradingDay()
{
	printmlb("api gettradingday");
	return api->GetTradingDay();
}

void CMdUserApi::RegisterFront(char *pszFrontAddress) {
	printmlb("api register front: %s", pszFrontAddress);
	api->RegisterFront(pszFrontAddress);
}

void CMdUserApi::RegisterNameServer(char *pszNsAddress) {
	printmlb("api register name server: %s", pszNsAddress);
	api->RegisterNameServer(pszNsAddress);
}

void CMdUserApi::RegisterFensUserInfo(CThostFtdcFensUserInfoField *pFensUserInfo) {
	printmlb("api register fens user info");
	printmlc("pFensUserInfo->BrokerID: %s", pFensUserInfo->BrokerID);
	printmlc("pFensUserInfo->UserID: %s", pFensUserInfo->UserID);
	printmlc("pFensUserInfo->LoginMode: %d", pFensUserInfo->LoginMode);
	api->RegisterFensUserInfo(pFensUserInfo);
}

void CMdUserApi::RegisterSpi() {
	printmlb("api register spi");
	api->RegisterSpi(this);
}

int CMdUserApi::SubscribeMarketData(char *ppInstrumentID[], int nCount) {
	printmlb("api sub market data");
	int i;
	for (i = 0; i < nCount; ++i) {
		printmlc("ppInstrumentID[%d]: %s", i, ppInstrumentID[i]);
	}
	return api->SubscribeMarketData(ppInstrumentID, nCount);
}

int CMdUserApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount) {
	printmlb("api un sub market data");
	int i;
	for (i = 0; i < nCount; ++i) {
		printmlc("ppInstrumentID[%d]: %s", i, ppInstrumentID[i]);
	}
	return api->UnSubscribeMarketData(ppInstrumentID, nCount);
}

int CMdUserApi::ReqUserLogin()
{
	printmlb("api req user login");

	CThostFtdcReqUserLoginField request = {};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));
	printmlc("request.BrokerID: %s", request.BrokerID);
	printmlc("request.UserID: %s", request.UserID);
	printmlc("request.Password: %s", request.Password);

	return api->ReqUserLogin(&request,++m_nRequestID);
}

int CMdUserApi::ReqUserLogout() {
	printmlb("api req user logout");

	CThostFtdcUserLogoutField request = {};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	printmlc("request.BrokerID: %s", request.BrokerID);
	printmlc("request.UserID: %s", request.UserID);

	return api->ReqUserLogout(&request,++m_nRequestID);
}

/***9 callback functions********************************************************************************/
//callback when connect successful.
//connect successful, and ReqUserLogin.
void CMdUserApi::OnFrontConnected()
{
	printmlb("connected successfully.");
	if (m_fnOnFrontConnected != NULL) {
		(*m_fnOnFrontConnected)(this);
	}
	//login automatically after connected.
	printmlb("request user login directly from here");
	ReqUserLogin();
}

//callback when connect unsuccessful.
void CMdUserApi::OnFrontDisconnected(int nReason)
{
	printmlb("connected failed, nReason: %d", nReason);
	if (m_fnOnFrontDisconnected != NULL) {
		(*m_fnOnFrontDisconnected)(this, nReason);
	}
}

void CMdUserApi::OnHeartBeatWarning(int nTimeLapse) {
	printmlb("OnHeartBeatWarning called, nTimeLapse: %d", nTimeLapse);
	if (m_fnOnHeartBeatWarning != NULL) {
		(*m_fnOnHeartBeatWarning)(this, nTimeLapse);
	}
}

//callback for ReqUserLogin.
void CMdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printmlb("OnRspUserLogin called.");
	printmlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printmlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printmlc("pRspInfo is NULL");
	}
	if (pRspUserLogin != NULL) {
		printmlc("pRspUserLogin->TradingDay: %s", pRspUserLogin->TradingDay);
		printmlc("pRspUserLogin->LoginTime: %s", pRspUserLogin->LoginTime);
		printmlc("pRspUserLogin->BrokerID: %s", pRspUserLogin->BrokerID);
		printmlc("pRspUserLogin->UserID: %s", pRspUserLogin->UserID); 
		printmlc("pRspUserLogin->SystemName: %s", pRspUserLogin->SystemName);
		printmlc("pRspUserLogin->FrontID: %d", pRspUserLogin->FrontID);
		printmlc("pRspUserLogin->SessionID: %d", pRspUserLogin->SessionID);
		printmlc("pRspUserLogin->MaxOrderRef: %s", pRspUserLogin->MaxOrderRef);
		printmlc("pRspUserLogin->SHFETime: %s", pRspUserLogin->SHFETime); 
		printmlc("pRspUserLogin->DCETime: %s", pRspUserLogin->DCETime);
		printmlc("pRspUserLogin->CZCETime: %s", pRspUserLogin->CZCETime);
		printmlc("pRspUserLogin->FFEXTime: %s", pRspUserLogin->FFEXTime);
		printmlc("pRspUserLogin->INETime: %s", pRspUserLogin->INETime);
	}
	else {
		printmlc("pRspUserLogin is NULL");
	}
	if (m_fnOnRspUserLogin != NULL) {
		(*m_fnOnRspUserLogin)(this, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
	}
	if ((pRspInfo==NULL || pRspInfo->ErrorID == 0) && pRspUserLogin) {
		printmlb("SubscribeMarketData from here");
		api->SubscribeMarketData(m_InstrumentIDs, m_InstrumentNum);
		//printmlb("SubscribeForQuoteRsp from here");
		//api->SubscribeForQuoteRsp(m_InstrumentIDs, m_InstrumentNum);
	}
}

void CMdUserApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printmlb("OnRspUserLogout called.");
	printmlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printmlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printmlc("pRspInfo is NULL");
	}
	if (pUserLogout != NULL) {
		printmlc("pUserLogout->BrokerID: %s, pUserLogout->UserID: %s", pUserLogout->BrokerID, pUserLogout->UserID);
	}
	else {
		printmlc("pUserLogout is NULL");
	}
	if (m_fnOnRspUserLogout != NULL) {
		(*m_fnOnRspUserLogout)(this, pUserLogout, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printmlb("OnRspError called.");
	printmlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printmlc("pRspInfo->ErrorID: %d, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printmlc("pRspInfo is NULL");
	}
	if (m_fnOnRspError != NULL) {
		(*m_fnOnRspError)(this, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printmlb("OnRspSubMarketData called.");
	printmlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printmlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printmlc("pRspInfo is NULL");
	}
	if (pSpecificInstrument != NULL) {
		printmlc("pSpecificInstrument->InstrumentID: %s", pSpecificInstrument->InstrumentID);
	}
	else {
		printmlc("pSpecificInstrument is NULL");
	}
	if (m_fnOnRspSubMarketData != NULL) {
		(*m_fnOnRspSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printmlb("OnRspUnSubMarketData called.");
	printmlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printmlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printmlc("pRspInfo is NULL");
	}
	if (pSpecificInstrument != NULL) {
		printmlc("pSpecificInstrument->InstrumentID: %s", pSpecificInstrument->InstrumentID);
	}
	else {
		printmlc("pSpecificInstrument is NULL");
	}
	if (m_fnOnRspUnSubMarketData != NULL) {
		(*m_fnOnRspUnSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	printmlb("DepthMarketData comming");
	printmld("pDepthMarketData->TradingDay: %s", pDepthMarketData->TradingDay);
	printmld("pDepthMarketData->InstrumentID: %s", pDepthMarketData->InstrumentID);
	printmld("pDepthMarketData->ExchangeID: %s", pDepthMarketData->ExchangeID);
	printmld("pDepthMarketData->LastPrice: %f", pDepthMarketData->LastPrice);
	printmld("pDepthMarketData->PreSettlementPrice: %f", pDepthMarketData->PreSettlementPrice);
	printmld("pDepthMarketData->PreClosePrice: %f", pDepthMarketData->PreClosePrice);
	printmld("pDepthMarketData->PreOpenInterest: %f", pDepthMarketData->PreOpenInterest);
	printmld("pDepthMarketData->OpenPrice: %f", pDepthMarketData->OpenPrice);
	printmld("pDepthMarketData->HighestPrice: %f", pDepthMarketData->HighestPrice);
	printmld("pDepthMarketData->LowestPrice: %f", pDepthMarketData->LowestPrice);
	printmld("pDepthMarketData->Volume: %d", pDepthMarketData->Volume);
	printmld("pDepthMarketData->Turnover: %f", pDepthMarketData->Turnover);
	printmld("pDepthMarketData->OpenInterest: %f", pDepthMarketData->OpenInterest);
	printmld("pDepthMarketData->ClosePrice: %f", pDepthMarketData->ClosePrice);
	printmld("pDepthMarketData->SettlementPrice: %f", pDepthMarketData->SettlementPrice);
	printmld("pDepthMarketData->UpperLimitPrice: %f", pDepthMarketData->UpperLimitPrice);
	printmld("pDepthMarketData->LowerLimitPrice: %f", pDepthMarketData->LowerLimitPrice);
	printmld("pDepthMarketData->PreDelta: %f", pDepthMarketData->PreDelta);
	printmld("pDepthMarketData->CurrDelta: %f", pDepthMarketData->CurrDelta);
	printmld("pDepthMarketData->UpdateTime: %s", pDepthMarketData->UpdateTime);
	printmld("pDepthMarketData->UpdateMillisec: %d", pDepthMarketData->UpdateMillisec);
	printmld("pDepthMarketData->BidPrice1: %f", pDepthMarketData->BidPrice1);
	printmld("pDepthMarketData->BidVolume1: %d", pDepthMarketData->BidVolume1);
	printmld("pDepthMarketData->AskPrice1: %f", pDepthMarketData->AskPrice1);
	printmld("pDepthMarketData->AskVolume1: %d", pDepthMarketData->AskVolume1);
	printmld("pDepthMarketData->BidPrice2: %f", pDepthMarketData->BidPrice2);
	printmld("pDepthMarketData->BidVolume2: %d", pDepthMarketData->BidVolume2);
	printmld("pDepthMarketData->AskPrice2: %f", pDepthMarketData->AskPrice2);
	printmld("pDepthMarketData->AskVolume2: %d", pDepthMarketData->AskVolume2);
	printmld("pDepthMarketData->BidPrice3: %f", pDepthMarketData->BidPrice3);
	printmld("pDepthMarketData->BidVolume3: %d", pDepthMarketData->BidVolume3);
	printmld("pDepthMarketData->AskPrice3: %f", pDepthMarketData->AskPrice3);
	printmld("pDepthMarketData->AskVolume3: %d", pDepthMarketData->AskVolume3);
	printmld("pDepthMarketData->BidPrice4: %f", pDepthMarketData->BidPrice4);
	printmld("pDepthMarketData->BidVolume4: %d", pDepthMarketData->BidVolume4);
	printmld("pDepthMarketData->AskPrice4: %f", pDepthMarketData->AskPrice4);
	printmld("pDepthMarketData->AskVolume4: %d", pDepthMarketData->AskVolume4);
	printmld("pDepthMarketData->BidPrice5: %f", pDepthMarketData->BidPrice5);
	printmld("pDepthMarketData->BidVolume5: %d", pDepthMarketData->BidVolume5);
	printmld("pDepthMarketData->AskPrice5: %f", pDepthMarketData->AskPrice5);
	printmld("pDepthMarketData->AskVolume5: %d", pDepthMarketData->AskVolume5);
	printmld("pDepthMarketData->AveragePrice: %f", pDepthMarketData->AveragePrice);
	printmld("pDepthMarketData->ActionDay: %s", pDepthMarketData->ActionDay);
	if (m_fnOnRtnDepthMarketData != NULL) {
		(*m_fnOnRtnDepthMarketData)(this, pDepthMarketData);
	}
	input_DMDQ(pDepthMarketData);
}

/***help functions**********************************************************************************/
void CMdUserApi::input_DMDQ(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	struct timespec tv;
	clock_gettime(CLOCK_REALTIME, &tv);
	long ts = tv.tv_sec;
	long tns = tv.tv_nsec;
	m_queue[m_tail_index]=*pDepthMarketData;
	m_intime_second[m_tail_index] = ts;
	m_intime_usecond[m_tail_index] = tns;
	m_tail_index = (m_tail_index+1)&(m_queue_size-1);
	m_current_size[m_tail_index] = __sync_add_and_fetch(&m_validmsg_size, 1);
}

CThostFtdcDepthMarketDataField *CMdUserApi::output_DMDQ(long *ts, long *tns, int *size) {
	if (m_validmsg_size> 0) {
		__sync_fetch_and_sub(&m_validmsg_size, 1);
		*ts = m_intime_second[m_header_index];
		*tns = m_intime_usecond[m_header_index];
		*size = m_current_size[m_header_index];
		CThostFtdcDepthMarketDataField *h = m_queue + m_header_index;
		m_header_index = (m_header_index + 1)&(m_queue_size-1);
		return h;
	}
	return NULL;
}
