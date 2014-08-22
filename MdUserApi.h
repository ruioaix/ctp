#ifndef CTP_C_API_MDUSERAPI_H
#define CTP_C_API_MDUSERAPI_H

#include "header.h"
#include "cpp_api/ThostFtdcMdApi.h"

#include <set>
#include <string>
#include <atomic>
#include <mutex>

using namespace std;

class CMdUserApi :
	public CThostFtdcMdSpi
{
public:
	CMdUserApi(void);
	virtual ~CMdUserApi(void);

	void Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword);
	void Disconnect();

	void Subscribe(const string& szInstrumentIDs);
	void Unsubscribe(const string& szInstrumentIDs);

	void SubscribeQuote(const string& szInstrumentIDs);
	void UnsubscribeQuote(const string& szInstrumentIDs);

private:
	//��������
	void Subscribe(const set<string>& instrumentIDs);
	void SubscribeQuote(const set<string>& instrumentIDs);
	//��¼����
	void ReqUserLogin();

	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

	//����Ƿ����
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//��������Ϣ�͵���Ϣ����
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//���ͳ�����Ϣ


public:
	//set callback member
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

	atomic<int>					m_nRequestID;			//����ID��ÿ������ǰ����
	
	set<string>					m_setInstrumentIDs;		//���ڶ��ĵĺ�Լ
	set<string>					m_setQuoteInstrumentIDs;		//���ڶ��ĵĺ�Լ
	CThostFtdcMdApi*			m_pApi;					//����API

	string						m_szPath;				//���������ļ���·��
	set<string>					m_arrAddresses;			//��������ַ
	string						m_szBrokerId;			//����ID
	string						m_szInvestorId;			//Ͷ����ID
	string						m_szPassword;			//����

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
