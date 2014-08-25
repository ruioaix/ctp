#ifndef CTP_C_API_MDUSERAPI_H
#define CTP_C_API_MDUSERAPI_H

#include "ctpcapi.h"
#include "cpp_api/ThostFtdcMdApi.h"

#include <set>
#include <string>
#include <mutex>

using namespace std;

class CMdUserApi : public CThostFtdcMdSpi
{
public:
	//create api object. 
	CMdUserApi(char *flowpath, char *servername, char *bid, char *iid, char *pd);
	virtual ~CMdUserApi(void);

	void Init();
	void Join();
	void GetTradingDay();
	void RegisterFront(char *pszFrontAddress);
	void RegisterNameServer(char *pszNsAddress);
	void RegisterFensUserInfo(CThostFtdcFensUserInfoField *pFensUserInfo);
	void RegisterSpi();
	int SubscribeMarketData(char *ppInstrumentID[], int nCount);
	int UnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);
	int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);

	void Subscribe(const string& szInstrumentIDs);
	void Unsubscribe(const string& szInstrumentIDs);

	void SubscribeQuote(const string& szInstrumentIDs);
	void UnsubscribeQuote(const string& szInstrumentIDs);

	void ReqUserLogin();
private:
	//��������
	void Subscribe(const set<string>& instrumentIDs);
	void SubscribeQuote(const set<string>& instrumentIDs);
	//��¼����

	//����OnFrontDisconnected(int nReason)��ֵ���ϴ�����Ϣ
	void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

	//12 callback functions in CThostFtdcMdSpi
	//these functions will only be called by the CTP server.
	//the user defined callback function will be called in the following functions.
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnHeartBeatWarning(int nTimeLapse);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	//����Ƿ����
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//��������Ϣ�͵���Ϣ����
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//���ͳ�����Ϣ


public:
	//12 callback register functions 
	void RegisterCallback_ofc(fnOnFrontConnected pCallback) { m_fnOnFrontConnected = pCallback; }
	void RegisterCallback_ofd(fnOnFrontDisconnected pCallback) { m_fnOnFrontDisconnected = pCallback; }
	void RegisterCallback_ohb(fnOnHeartBeatWarning pCallback) { m_fnOnHeartBeatWarning = pCallback; }
	void RegisterCallback_oruli(fnOnRspUserLogin pCallback) { m_fnOnRspUserLogin = pCallback; }
	void RegisterCallback_orulo(fnOnRspUserLogout pCallback) { m_fnOnRspUserLogout = pCallback; }
	void RegisterCallback_ore(fnOnRspError pCallback) { m_fnOnRspError = pCallback; }
	void RegisterCallback_orsmd(fnOnRspSubMarketData pCallback) { m_fnOnRspSubMarketData = pCallback; }
	void RegisterCallback_orusmd(fnOnRspUnSubMarketData pCallback) { m_fnOnRspUnSubMarketData = pCallback; }
	void RegisterCallback_orsfqr(fnOnRspSubForQuoteRsp pCallback) { m_fnOnRspSubForQuoteRsp = pCallback; }
	void RegisterCallback_orusfqr(fnOnRspUnSubForQuoteRsp pCallback) { m_fnOnRspUnSubForQuoteRsp = pCallback; }
	void RegisterCallback_ordmd(fnOnRtnDepthMarketData pCallback) { m_fnOnRtnDepthMarketData = pCallback; }
	void RegisterCallback_orfqr(fnOnRtnForQuoteRsp pCallback) { m_fnOnRtnForQuoteRsp = pCallback; }

private:
	mutex						m_csMapInstrumentIDs;
	mutex						m_csMapQuoteInstrumentIDs;

	int							m_nRequestID;
	
	set<string>					m_setInstrumentIDs;		//���ڶ��ĵĺ�Լ
	set<string>					m_setQuoteInstrumentIDs;		//���ڶ��ĵĺ�Լ
	CThostFtdcMdApi*			m_pApi;					//����API

	set<string>					m_arrAddresses;			//��������ַ
	char *m_szPath;				//���������ļ���·��
	char *m_server;
	char *m_szBrokerId;			//����ID
	char *m_szInvestorId;			//Ͷ����ID
	char *m_szPassword;			//����

	fnOnFrontConnected m_fnOnFrontConnected;
	fnOnFrontDisconnected m_fnOnFrontDisconnected;
	fnOnHeartBeatWarning m_fnOnHeartBeatWarning;
	fnOnRspUserLogin m_fnOnRspUserLogin;
	fnOnRspUserLogout m_fnOnRspUserLogout;
	fnOnRspError m_fnOnRspError;
	fnOnRspSubMarketData m_fnOnRspSubMarketData;
	fnOnRspUnSubMarketData m_fnOnRspUnSubMarketData;
	fnOnRspSubForQuoteRsp m_fnOnRspSubForQuoteRsp;
	fnOnRspUnSubForQuoteRsp m_fnOnRspUnSubForQuoteRsp;
	fnOnRtnDepthMarketData m_fnOnRtnDepthMarketData;
	fnOnRtnForQuoteRsp m_fnOnRtnForQuoteRsp;
	
};

#endif
