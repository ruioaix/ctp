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
		m_pApi->RegisterNameServer(m_server);
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
	Disconnect();
	if (m_pApi) {
		m_pApi->Release();
	}
}

/***special function to simply connect&disconnect process*****************************************************/
//connect: include CreateFtdcMdApi, RegisterSpi, RegisterFront, Init.
void CMdUserApi::Connect()
{
	if (m_pApi) {
		m_pApi->Init();
	}
}

void CMdUserApi::Disconnect() {
	if(m_pApi) {
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;
	}
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
	
	//ֻ����һ���õ���m_nRequestID��û�б�Ҫÿ������m_nRequestID����0��ʼ
	m_pApi->ReqUserLogin(&request,++m_nRequestID);
}

/***12 callback functions********************************************************************************/
//callback when connect successful.
//connect successful, and ReqUserLogin.
void CMdUserApi::OnFrontConnected()
{
	//���ӳɹ����Զ������¼
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
	//����ʧ�ܷ��ص���Ϣ��ƴ�Ӷ��ɣ���Ҫ��Ϊ��ͳһ���
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
		//�п��ܶ����ˣ������Ƕ������������¶���
		set<string> mapOld = m_setInstrumentIDs;//�����ϴζ��ĵĺ�Լ
		//Unsubscribe(mapOld);//�����Ѿ������ˣ�û�б�Ҫ��ȡ������
		Subscribe(mapOld);//����

		//�п��ܶ����ˣ������Ƕ������������¶���
		mapOld = m_setQuoteInstrumentIDs;//�����ϴζ��ĵĺ�Լ
		SubscribeQuote(mapOld);//����
	}
	else
	{
		//m_status = E_authed;
		//if(m_msgQueue)
		//	m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
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
	printf("Sub MD done.\n");
	//��ģ��ƽ̨��������������ᴥ��������Ҫ�Լ�ά��һ���Ѿ����ĵĺ�Լ�б�
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
	//ģ��ƽ̨��������������ᴥ��
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
		strcpy(pRspInfo->ErrorMsg,"0x1001 �����ʧ��");
		break;
	case 0x1002:
		strcpy(pRspInfo->ErrorMsg,"0x1002 ����дʧ��");
		break;
	case 0x2001:
		strcpy(pRspInfo->ErrorMsg,"0x2001 ����������ʱ");
		break;
	case 0x2002:
		strcpy(pRspInfo->ErrorMsg,"0x2002 ��������ʧ��");
		break;
	case 0x2003:
		strcpy(pRspInfo->ErrorMsg,"0x2003 �յ�������");
		break;
	default:
		sprintf(pRspInfo->ErrorMsg,"%x δ֪����", pRspInfo->ErrorID);
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
			//��Լ�Ѿ����ڣ������ٶ��ģ�����ζ���Ҳû��ϵ
			m_setInstrumentIDs.insert(token);//��¼�˺�Լ���ж���
			vct.push_back(token);
		}
		token = strtok( NULL, delimiter);
	}
	
	if(vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->SubscribeMarketData(pArray,(int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
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
			//��Լ�Ѿ������ڣ�������ȡ�����ģ������ȡ��Ҳûʲô��ϵ
			m_setInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok( NULL, delimiter);
	}
	
	if(vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->UnSubscribeMarketData(pArray,(int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
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
			//��Լ�Ѿ����ڣ������ٶ��ģ�����ζ���Ҳû��ϵ
			m_setQuoteInstrumentIDs.insert(token);//��¼�˺�Լ���ж���
			vct.push_back(token);
		}
		token = strtok(NULL, delimiter);
	}

	if (vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->SubscribeForQuoteRsp(pArray, (int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
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
			//��Լ�Ѿ������ڣ�������ȡ�����ģ������ȡ��Ҳûʲô��ϵ
			m_setQuoteInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok(NULL, delimiter);
	}

	if (vct.size()>0)
	{
		//ת���ַ�������
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//����
		m_pApi->UnSubscribeForQuoteRsp(pArray, (int)vct.size());

		delete[] pArray;
	}

	//�ͷ��ڴ�
	delete[] buf;
}

