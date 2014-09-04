#include "mduserapi.h"

#include <string.h>
#include <iostream>
#include <vector>
#include <mutex>
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
	printlb("create md.\n");
	m_pApi = NULL;
	m_nRequestID = 0;

	m_szPath = flowpath;
	m_server = servername;
	m_szBrokerId = brokerid;
	m_szInvestorId = inverstorid;
	m_szPassword = password;

	m_InstrumentIDs = InstrumentIDs;
	m_InstrumentNum = InstrumentNum;

	mkdir(m_szPath, 0777);
	m_pApi = CThostFtdcMdApi::CreateFtdcMdApi(m_szPath);
	if (m_pApi == NULL) {
		isError("mdapi created failed");
	}
	m_pApi->RegisterSpi(this);
	m_pApi->RegisterFront(m_server);

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
	int i;
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
	m_pApi->RegisterSpi(NULL);
	m_pApi->Release();
	m_pApi = NULL;
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
	m_pApi->Init();
}

void CMdUserApi::Join()
{
	printlb("api join");
	m_pApi->Join();
}

const char *CMdUserApi::GetTradingDay()
{
	printlb("api gettradingday");
	return m_pApi->GetTradingDay();
}

void CMdUserApi::RegisterFront(char *pszFrontAddress) {
	printlb("api register front");
	m_pApi->RegisterFront(pszFrontAddress);
}

void CMdUserApi::RegisterNameServer(char *pszNsAddress) {
	printlb("api register name server");
	m_pApi->RegisterNameServer(pszNsAddress);
}

void CMdUserApi::RegisterFensUserInfo(CThostFtdcFensUserInfoField *pFensUserInfo) {
	printlb("api register fens user info");
	m_pApi->RegisterFensUserInfo(pFensUserInfo);
}

void CMdUserApi::RegisterSpi() {
	printlb("api register spi");
	m_pApi->RegisterSpi(this);
}

int CMdUserApi::SubscribeMarketData(char *ppInstrumentID[], int nCount) {
	printlb("api sub market data");
	return m_pApi->SubscribeMarketData(ppInstrumentID, nCount);
}

int CMdUserApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount) {
	printlb("api un sub market data");
	return m_pApi->UnSubscribeMarketData(ppInstrumentID, nCount);
}

int CMdUserApi::SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {
	printlb("api sub quota rsp");
	return m_pApi->SubscribeForQuoteRsp(ppInstrumentID, nCount);
}

int CMdUserApi::UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {
	printlb("api un sub quota rsp");
	return m_pApi->UnSubscribeForQuoteRsp(ppInstrumentID, nCount);
}

int CMdUserApi::ReqUserLogin()
{
	printlb("api req user login");

	CThostFtdcReqUserLoginField request = {0};
	strncpy(request.BrokerID, m_szBrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_szPassword, sizeof(TThostFtdcPasswordType));
	printlc("request.BrokerID: %s", request.BrokerID);
	printlc("request.UserID: %s", request.UserID);
	printlc("request.Password: %s", request.Password);

	return m_pApi->ReqUserLogin(&request,++m_nRequestID);
}

int CMdUserApi::ReqUserLogout() {
	printlb("api req user logout");

	CThostFtdcUserLogoutField request = {0};
	strncpy(request.BrokerID, m_szBrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId, sizeof(TThostFtdcInvestorIDType));
	printlc("request.BrokerID: %s", request.BrokerID);
	printlc("request.UserID: %s", request.UserID);

	return m_pApi->ReqUserLogout(&request,++m_nRequestID);
}

/***12 callback functions********************************************************************************/
//callback when connect successful.
//connect successful, and ReqUserLogin.
void CMdUserApi::OnFrontConnected()
{
	if (m_fnOnFrontConnected != NULL) {
		(*m_fnOnFrontConnected)(this);
	}
	//login automatically after connected.
	ReqUserLogin();
}

//callback when connect unsuccessful.
void CMdUserApi::OnFrontDisconnected(int nReason)
{
	if (m_fnOnFrontDisconnected != NULL) {
		(*m_fnOnFrontDisconnected)(this, nReason);
	}
	CThostFtdcRspInfoField RspInfo;
	RspInfo.ErrorID = nReason;
	GetOnFrontDisconnectedMsg(&RspInfo);
}

void CMdUserApi::OnHeartBeatWarning(int nTimeLapse) {
	if (m_fnOnHeartBeatWarning != NULL) {
		(*m_fnOnHeartBeatWarning)(this, nTimeLapse);
	}
}

//callback for ReqUserLogin.
void CMdUserApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUserLogin != NULL) {
		(*m_fnOnRspUserLogin)(this, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
	}
	if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin) {
		m_pApi->SubscribeMarketData(m_InstrumentIDs, m_InstrumentNum);
	}
}

void CMdUserApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (m_fnOnRspUserLogout != NULL) {
		(*m_fnOnRspUserLogout)(this, pUserLogout, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspError != NULL) {
		(*m_fnOnRspError)(this, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspSubMarketData != NULL) {
		(*m_fnOnRspSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUnSubMarketData != NULL) {
		(*m_fnOnRspUnSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (m_fnOnRspSubForQuoteRsp != NULL) {
		(*m_fnOnRspSubForQuoteRsp)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUnSubForQuoteRsp) {
		(*m_fnOnRspUnSubForQuoteRsp)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (m_fnOnRtnDepthMarketData != NULL) {
		(*m_fnOnRtnDepthMarketData)(this, pDepthMarketData);
	}
	input_DMDQ(pDepthMarketData);
}

void CMdUserApi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
	if (m_fnOnRtnForQuoteRsp != NULL) {
		(*m_fnOnRtnForQuoteRsp)(this, pForQuoteRsp);
	}
}

/***help functions**********************************************************************************/
bool CMdUserApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	return bRet;
}

bool CMdUserApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	return bRet;
}

void CMdUserApi::GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo)
{
	if(NULL == pRspInfo)
		return;

	switch(pRspInfo->ErrorID)
	{
		case 0x1001:
			strcpy(pRspInfo->ErrorMsg,"0x1001 network read failed");
			break;
		case 0x1002:
			strcpy(pRspInfo->ErrorMsg,"0x1002 network write failed");
			break;
		case 0x2001:
			strcpy(pRspInfo->ErrorMsg,"0x2001 receive heartbeat timeout");
			break;
		case 0x2002:
			strcpy(pRspInfo->ErrorMsg,"0x2002 send heartbeat failed");
			break;
		case 0x2003:
			strcpy(pRspInfo->ErrorMsg,"0x2003 receive error msg");
			break;
		default:
			sprintf(pRspInfo->ErrorMsg,"%x unkown error", pRspInfo->ErrorID);
			break;
	}
}

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
