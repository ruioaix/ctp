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

#define delimiter ",;"

using namespace std;

/***create and delete************************************************************************************/
//create
CMdUserApi::CMdUserApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, char ** InstrumentIDs, int InstrumentNum)
{
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
	if (m_pApi) {
		m_pApi->RegisterSpi(this);
		m_pApi->RegisterFront(m_server);
	}

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


	loopL = 1024;
	queue = (CThostFtdcDepthMarketDataField *)malloc(loopL * sizeof(CThostFtdcDepthMarketDataField));
	intime_ts = (long *)malloc(loopL * sizeof(long));
	intime_tus = (int *)malloc(loopL * sizeof(int));
	int i;
	for (i = 0; i < loopL; ++i) {
		intime_ts[i] = -1;
		intime_tus[i] = -1;
	}
	itm = 0;
	otm = 0;
	header = queue;
	tail = queue;
	hi = 0;
	ti = 0;
	size = 0;
	pthread_mutex_init(&hasValue_mutex, NULL);
	pthread_cond_init(&hasValue_cond, NULL);
	running = 1;
}

//delete
CMdUserApi::~CMdUserApi(void)
{
	if(m_pApi) {
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
	pthread_mutex_destroy(&hasValue_mutex);
	pthread_cond_destroy(&hasValue_cond);
	free(queue);
	free(intime_ts);
	free(intime_tus);
}

/***13 functions, merge api functions in MdApi class to MdSpi class****************************************************/
//connect: include CreateFtdcMdApi, RegisterSpi, RegisterFront, Init.
void CMdUserApi::Init()
{
	if (m_pApi) {
		m_pApi->Init();
	}
}

void CMdUserApi::Join()
{
	if (m_pApi) {
		m_pApi->Join();
	}
}

void CMdUserApi::GetTradingDay()
{
	if (m_pApi) {
		m_pApi->GetTradingDay();
	}
}

void CMdUserApi::RegisterFront(char *pszFrontAddress) {
	if (m_pApi) {
		m_pApi->RegisterFront(pszFrontAddress);
	}
}

void CMdUserApi::RegisterNameServer(char *pszNsAddress) {
	if (m_pApi) {
		m_pApi->RegisterNameServer(pszNsAddress);
	}
}

void CMdUserApi::RegisterFensUserInfo(CThostFtdcFensUserInfoField *pFensUserInfo) {
	if (m_pApi) {
		m_pApi->RegisterFensUserInfo(pFensUserInfo);
	}
}

void CMdUserApi::RegisterSpi() {
	if (m_pApi) {
		m_pApi->RegisterSpi(this);
	}
}

int CMdUserApi::SubscribeMarketData(char *ppInstrumentID[], int nCount) {
	if (m_pApi) {
		return m_pApi->SubscribeMarketData(ppInstrumentID, nCount);
	}
	return -1;
}

int CMdUserApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount) {
	if (m_pApi) {
		return m_pApi->UnSubscribeMarketData(ppInstrumentID, nCount);
	}
	return -1;
}

int CMdUserApi::SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {
	if (m_pApi) {
		return m_pApi->SubscribeForQuoteRsp(ppInstrumentID, nCount);
	}
	return -1;
}

int CMdUserApi::UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount) {
	if (m_pApi) {
		return m_pApi->UnSubscribeForQuoteRsp(ppInstrumentID, nCount);
	}
	return -1;
}

int CMdUserApi::ReqUserLogin()
{
	if (NULL == m_pApi) return -1;

	CThostFtdcReqUserLoginField request = {0};
	strncpy(request.BrokerID, m_szBrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_szPassword, sizeof(TThostFtdcPasswordType));

	//只有这一处用到了m_nRequestID，没有必要每次重连m_nRequestID都从0开始
	return m_pApi->ReqUserLogin(&request,++m_nRequestID);
}

int CMdUserApi::ReqUserLogout() {
	if (NULL == m_pApi) return -1;

	CThostFtdcUserLogoutField request = {0};
	strncpy(request.BrokerID, m_szBrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId, sizeof(TThostFtdcInvestorIDType));

	return m_pApi->ReqUserLogout(&request,++m_nRequestID);
}

/***12 callback functions********************************************************************************/
//callback when connect successful.
//connect successful, and ReqUserLogin.
void CMdUserApi::OnFrontConnected()
{
	//连接成功后自动请求登录
	if (m_fnOnFrontConnected != NULL) {
		(*m_fnOnFrontConnected)(this);
	}
	ReqUserLogin();
}

//callback when connect unsuccessful.
void CMdUserApi::OnFrontDisconnected(int nReason)
{
	if (m_fnOnFrontDisconnected != NULL) {
		(*m_fnOnFrontDisconnected)(this, nReason);
	}
	CThostFtdcRspInfoField RspInfo;
	//连接失败返回的信息是拼接而成，主要是为了统一输出
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
	printf("Sub_MD_done.\n");
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUnSubMarketData != NULL) {
		(*m_fnOnRspUnSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
	printf("UnSub MD done.\n");
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
	struct timeval tv;
	gettimeofday (&tv, NULL);
	long ts = tv.tv_sec;
    int tus = tv.tv_usec;
	input_DMDQ(pDepthMarketData, ts, tus);
	if (m_fnOnRtnDepthMarketData != NULL) {
		(*m_fnOnRtnDepthMarketData)(this, pDepthMarketData);
	}
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
			strcpy(pRspInfo->ErrorMsg,"0x1001 网络读失败");
			break;
		case 0x1002:
			strcpy(pRspInfo->ErrorMsg,"0x1002 网络写失败");
			break;
		case 0x2001:
			strcpy(pRspInfo->ErrorMsg,"0x2001 接收心跳超时");
			break;
		case 0x2002:
			strcpy(pRspInfo->ErrorMsg,"0x2002 发送心跳失败");
			break;
		case 0x2003:
			strcpy(pRspInfo->ErrorMsg,"0x2003 收到错误报文");
			break;
		default:
			sprintf(pRspInfo->ErrorMsg,"%x 未知错误", pRspInfo->ErrorID);
			break;
	}
}

void CMdUserApi::input_DMDQ(CThostFtdcDepthMarketDataField *pDepthMarketData, long ts, int tus) {
	queue[ti]=*pDepthMarketData;
	ti = (ti+1)&(loopL-1);
	tail++;

	
	intime_ts[itm] = ts;
	intime_tus[itm++] = tus;
	pid_t tid = syscall(SYS_gettid);
	pid_t pid = getpid();
	printf("tid: %d, pid: %d\n", tid, pid);
	printf("updated time: %s, update mill time : %4d, arrive: %ld.%06d\n", pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, ts, tus);
	if (tail == queue + loopL) tail = queue;
	__sync_fetch_and_add(&size, 1);
	printf("size_after_input_%d\n", size);
	pthread_mutex_lock(&hasValue_mutex);
	pthread_cond_signal(&hasValue_cond);
	pthread_mutex_unlock(&hasValue_mutex);
}

CThostFtdcDepthMarketDataField *CMdUserApi::output_DMDQ(long *ts, int *tus) {
	if (size > 0) {
		__sync_fetch_and_sub(&size, 1);
		*ts = intime_ts[otm];
		*tus = intime_tus[otm++];
		CThostFtdcDepthMarketDataField *h=queue + hi;
		hi = (hi+1)&(loopL-1);
		printf("size_after_output_%d\n", size);
		return h;
	}
	return NULL;
}
