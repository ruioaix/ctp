#include "mduserapi.h"

#include <string.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <sys/stat.h>

#define delimiter ",;"

using namespace std;

/***create and delete************************************************************************************/
//create
CMdUserApi::CMdUserApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password)
{
	m_pApi = NULL;
	m_nRequestID = 0;

	m_szPath = flowpath;
	m_server = servername;
	m_szBrokerId = brokerid;
	m_szInvestorId = inverstorid;
	m_szPassword = password;

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
}

//delete
CMdUserApi::~CMdUserApi(void)
{
	if(m_pApi) {
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
}

/***special function to simply connect&disconnect process*****************************************************/
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

void CMdUserApi::ReqUserLogin()
{
	if (NULL == m_pApi) {
		return;
	}

	CThostFtdcReqUserLoginField request = {0};
	
	strncpy(request.BrokerID, m_szBrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_szPassword, sizeof(TThostFtdcPasswordType));
	
	//只有这一处用到了m_nRequestID，没有必要每次重连m_nRequestID都从0开始
	m_pApi->ReqUserLogin(&request,++m_nRequestID);
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
	printf("ErrorId: %d, ErrorMsg: %s\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin) {
		//有可能断线了，本处是断线重连后重新订阅
		set<string> mapOld = m_setInstrumentIDs;//记下上次订阅的合约
		//Unsubscribe(mapOld);//由于已经断线了，没有必要再取消订阅
		Subscribe(mapOld);//订阅

		//有可能断线了，本处是断线重连后重新订阅
		mapOld = m_setQuoteInstrumentIDs;//记下上次订阅的合约
		SubscribeQuote(mapOld);//订阅

		//char *i1[2] = {"IF1409", "IF1410"};
		//m_pApi->SubscribeMarketData(i1, 2);
		//m_pApi->SubscribeForQuoteRsp(i1, 2);
	}
	//else
	//{
	//	//m_status = E_authed;
	//	//if(m_msgQueue)
	//	//	m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
	//}
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
	printf("Sub MD done.\n");
	if (m_fnOnRspSubMarketData != NULL) {
		(*m_fnOnRspSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
	//在模拟平台可能这个函数不会触发，所以要自己维护一张已经订阅的合约列表
	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast) &&pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapInstrumentIDs);

		m_setInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUnSubMarketData != NULL) {
		(*m_fnOnRspUnSubMarketData)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
	//模拟平台可能这个函数不会触发
	printf("UnSub MD done.\n");
	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
		&&pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapInstrumentIDs);

		m_setInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	if (m_fnOnRspSubForQuoteRsp != NULL) {
		(*m_fnOnRspSubForQuoteRsp)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
	if (!IsErrorRspInfo(pRspInfo, nRequestID, bIsLast)
		&& pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

		m_setQuoteInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_fnOnRspUnSubForQuoteRsp) {
		(*m_fnOnRspUnSubForQuoteRsp)(this, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
	if (!IsErrorRspInfo(pRspInfo, nRequestID, bIsLast)
		&& pSpecificInstrument)
	{
		lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

		m_setQuoteInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
	}
}

void CMdUserApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
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

/***some help functions**********************************************************************************/
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

/********************************************************************************************************/
void CMdUserApi::Subscribe(const string& szInstrumentIDs)
{
	if(NULL == m_pApi)
		return;

	vector<char*> vct;

	size_t len = szInstrumentIDs.length()+1;
	char* buf = new char[len];
	strncpy(buf,szInstrumentIDs.c_str(),len);

	lock_guard<mutex> cl(m_csMapInstrumentIDs);

	char* token = strtok(buf, delimiter);
	while(token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//合约已经存在，不用再订阅，但多次订阅也没关系
			m_setInstrumentIDs.insert(token);//记录此合约进行订阅
			vct.push_back(token);
		}
		token = strtok( NULL, delimiter);
	}
	
	if(vct.size()>0)
	{
		//转成字符串数组
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//订阅
		m_pApi->SubscribeMarketData(pArray,(int)vct.size());
		printf("iiiii: %s\n", pArray[0]);
		printf("iiiii: %s\n", pArray[1]);
		printf("iiiii: %d\n", (int)vct.size());

		delete[] pArray;
	}

	//释放内存
	delete[] buf;
}

void CMdUserApi::Subscribe(const set<string>& instrumentIDs)
{
	if(NULL == m_pApi)
		return;

	string szInstrumentIDs;
	for(set<string>::iterator i=instrumentIDs.begin();i!=instrumentIDs.end();++i)
	{
		szInstrumentIDs.append(*i);
		szInstrumentIDs.append(";");
	}

	if (szInstrumentIDs.length()>1)
	{
		Subscribe(szInstrumentIDs);
	}
}

void CMdUserApi::Unsubscribe(const string& szInstrumentIDs)
{
	if(NULL == m_pApi)
		return;

	vector<char*> vct;
	size_t len = szInstrumentIDs.length()+1;
	char* buf = new char[len];
	strncpy(buf,szInstrumentIDs.c_str(),len);

	lock_guard<mutex> cl(m_csMapInstrumentIDs);

	char* token = strtok(buf, delimiter);
	while(token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//合约已经不存在，不用再取消订阅，但多次取消也没什么关系
			m_setInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok( NULL, delimiter);
	}
	
	if(vct.size()>0)
	{
		//转成字符串数组
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//订阅
		m_pApi->UnSubscribeMarketData(pArray,(int)vct.size());

		delete[] pArray;
	}

	//释放内存
	delete[] buf;
}

void CMdUserApi::SubscribeQuote(const string& szInstrumentIDs)
{
	if (NULL == m_pApi)
		return;

	vector<char*> vct;

	size_t len = szInstrumentIDs.length() + 1;
	char* buf = new char[len];
	strncpy(buf, szInstrumentIDs.c_str(), len);

	lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

	char* token = strtok(buf, delimiter);
	while (token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//合约已经存在，不用再订阅，但多次订阅也没关系
			m_setQuoteInstrumentIDs.insert(token);//记录此合约进行订阅
			vct.push_back(token);
		}
		token = strtok(NULL, delimiter);
	}

	if (vct.size()>0)
	{
		//转成字符串数组
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//订阅
		m_pApi->SubscribeForQuoteRsp(pArray, (int)vct.size());

		delete[] pArray;
	}

	//释放内存
	delete[] buf;
}

void CMdUserApi::SubscribeQuote(const set<string>& instrumentIDs)
{
	if (NULL == m_pApi)
		return;

	string szInstrumentIDs;
	for (set<string>::iterator i = instrumentIDs.begin(); i != instrumentIDs.end(); ++i)
	{
		szInstrumentIDs.append(*i);
		szInstrumentIDs.append(";");
	}

	if (szInstrumentIDs.length()>1)
	{
		SubscribeQuote(szInstrumentIDs);
	}
}

void CMdUserApi::UnsubscribeQuote(const string& szInstrumentIDs)
{
	if (NULL == m_pApi)
		return;

	vector<char*> vct;
	size_t len = szInstrumentIDs.length() + 1;
	char* buf = new char[len];
	strncpy(buf, szInstrumentIDs.c_str(), len);

	lock_guard<mutex> cl(m_csMapQuoteInstrumentIDs);

	char* token = strtok(buf, delimiter);
	while (token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//合约已经不存在，不用再取消订阅，但多次取消也没什么关系
			m_setQuoteInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok(NULL, delimiter);
	}

	if (vct.size()>0)
	{
		//转成字符串数组
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//订阅
		m_pApi->UnSubscribeForQuoteRsp(pArray, (int)vct.size());

		delete[] pArray;
	}

	//释放内存
	delete[] buf;
}

