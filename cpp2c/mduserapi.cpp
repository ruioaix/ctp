#include "mduserapi.h"

#include <cstring>
#include <ctime>

using namespace std;

/***create and delete************************************************************************************/
//create
CMdUserApi::CMdUserApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, char ** InstrumentIDs, int InstrumentNum)
{
	m_nRequestID = 0;
	m_logFilePath = flowpath;
	m_server = servername;
	m_BrokerId = brokerid;
	m_InvestorId = inverstorid;
	m_Password = password;
	m_InstrumentIDs = InstrumentIDs;
	m_InstrumentNum = InstrumentNum;
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
	m_queue = (CThostFtdcDepthMarketDataField *)malloc(m_queue_size * sizeof(CThostFtdcDepthMarketDataField));
	if (m_queue == NULL) {
		isError("mdapi m_queue malloc failed");
	}
	m_intime_second = (long *)malloc(m_queue_size * sizeof(long));
	if (m_intime_second == NULL) {
		isError("mdapi m_intime_second malloc failed");
	}
	m_intime_usecond = (long *)malloc(m_queue_size * sizeof(long));
	if (m_intime_usecond == NULL) {
		isError("mdapi m_intime_usecond malloc failed");
	}
	m_current_size = (int *)malloc(m_queue_size * sizeof(int));
	if (m_current_size == NULL) {
		isError("mdapi m_current_size malloc failed");
	}
	m_current_size[0]=0;
	int i;
	for (i = 0; i < m_queue_size; ++i) {
		m_intime_second[i] = -1;
		m_intime_usecond[i] = -1;
	}
	m_header_index_slow = 0;
	m_header_index = 0;
	m_tail_index = 0;
	m_validmsg_size = 0;
	m_validmsg_size_slow = 0;

	//verbose
	printmlb("create both spi and api object - md.");
	printmlc("save m_nRequestID: %d", m_nRequestID);
	printmlc("save m_logFilePath: %s", m_logFilePath);
	printmlc("save m_server: %s", m_server);
	printmlc("save m_BrokerId: %s", m_BrokerId);
	printmlc("save m_InvestorId: %s", m_InvestorId);
	for (i = 0; i < m_InstrumentNum; ++i) {
		printmlc("save Instrument %d: %s", i, m_InstrumentIDs[i]);
	}
	printmlc("prepare msg queue, size: %d", m_queue_size);
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
	printmlb("free md");
}

/***11 functions, merge api functions in MdApi class to MdSpi class****************************************************/
//connect: include CreateFtdcMdApi, RegisterSpi, RegisterFront, Init.
void CMdUserApi::Init()
{
	api->Init();
	printmlb("api init");
}

void CMdUserApi::Join()
{
	api->Join();
	printmlb("api join");
}

const char *CMdUserApi::GetTradingDay()
{
	printmlb("api gettradingday");
	return api->GetTradingDay();
}

void CMdUserApi::RegisterFront(char *pszFrontAddress) {
	api->RegisterFront(pszFrontAddress);
	printmlb("api register front: %s", pszFrontAddress);
}

void CMdUserApi::RegisterNameServer(char *pszNsAddress) {
	api->RegisterNameServer(pszNsAddress);
	printmlb("api register name server: %s", pszNsAddress);
}

void CMdUserApi::RegisterFensUserInfo() {
	CThostFtdcFensUserInfoField pFensUserInfo;
	//TODO prepare pFensUserInfo;
	api->RegisterFensUserInfo(&pFensUserInfo);

	printmlb("api register fens user info");
}

void CMdUserApi::RegisterSpi() {
	api->RegisterSpi(this);
	printmlb("api register spi");
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

	CThostFtdcReqUserLoginField request;
	memset(&request, 0, sizeof(CThostFtdcRspUserLoginField));
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcUserIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));

	printmlb("api req user login");
	printmlc("request.BrokerID: %s", request.BrokerID);
	printmlc("request.UserID: %s", request.UserID);

	return api->ReqUserLogin(&request,++m_nRequestID);
}

int CMdUserApi::ReqUserLogout() {

	CThostFtdcUserLogoutField request = {};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcUserIDType));

	printmlb("api req user logout");
	printmlc("request.BrokerID: %s", request.BrokerID);
	printmlc("request.UserID: %s", request.UserID);

	return api->ReqUserLogout(&request,++m_nRequestID);
}

/***9 callback functions********************************************************************************/
//callback when connect successful.
//connect successful, and ReqUserLogin.
void CMdUserApi::OnFrontConnected()
{
	if (m_fnOnFrontConnected != NULL) {
		(*m_fnOnFrontConnected)(this);
	}
	//login automatically after connected.
	ReqUserLogin();

	printmlb("connected successfully.");
	printmlb("request user login directly from here");
}

//callback when connect unsuccessful.
void CMdUserApi::OnFrontDisconnected(int nReason)
{
	if (m_fnOnFrontDisconnected != NULL) {
		(*m_fnOnFrontDisconnected)(this, nReason);
	}

	printmlb("connected failed, nReason: %d", nReason);
}

void CMdUserApi::OnHeartBeatWarning(int nTimeLapse) {
	if (m_fnOnHeartBeatWarning != NULL) {
		(*m_fnOnHeartBeatWarning)(this, nTimeLapse);
	}

	printmlb("OnHeartBeatWarning called, nTimeLapse: %d", nTimeLapse);
}

//callback for ReqUserLogin.
void CMdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUserLogin != NULL) {
		(*m_fnOnRspUserLogin)(this, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
	}
	if ((pRspInfo==NULL || pRspInfo->ErrorID == 0) && pRspUserLogin) {
		printmlb("SubscribeMarketData from here");
		api->SubscribeMarketData(m_InstrumentIDs, m_InstrumentNum);
	}

	//verbose
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
}

void CMdUserApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (m_fnOnRspUserLogout != NULL) {
		(*m_fnOnRspUserLogout)(this, pUserLogout, pRspInfo, nRequestID, bIsLast);
	}

	//verbose;
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
}

void CMdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspError != NULL) {
		(*m_fnOnRspError)(this, pRspInfo, nRequestID, bIsLast);
	}

	//verbose
	printmlb("OnRspError called.");
	printmlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printmlc("pRspInfo->ErrorID: %d, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printmlc("pRspInfo is NULL");
	}
}

void CMdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspSubMarketData != NULL) {
		(*m_fnOnRspSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}

	//verbose
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
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUnSubMarketData != NULL) {
		(*m_fnOnRspUnSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}

	//verbose
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
}

void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (m_fnOnRtnDepthMarketData != NULL) {
		(*m_fnOnRtnDepthMarketData)(this, pDepthMarketData);
	}
	input_DMDQ(pDepthMarketData);

	//verbose
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
	__sync_add_and_fetch(&m_validmsg_size_slow, 1);
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

CThostFtdcDepthMarketDataField *CMdUserApi::output_slow_DMDQ() {
	if (m_validmsg_size_slow> 0) {
		__sync_fetch_and_sub(&m_validmsg_size_slow, 1);
		CThostFtdcDepthMarketDataField *h = m_queue + m_header_index_slow;
		m_header_index_slow = (m_header_index_slow + 1)&(m_queue_size-1);
		return h;
	}
	return NULL;
}
