#ifndef CTP_C_API_MDUSERAPI_H
#define CTP_C_API_MDUSERAPI_H

#include "ctpcapi.h"
#include "cpp_api/ThostFtdcMdApi.h"

using namespace std;


class CMdUserApi : public CThostFtdcMdSpi
{

public:
	//there are 15 functions in CThostFtdcMdApi.
	//create api object. 2 api functions: CreateFtdcMdApi & Release are included in the following two.
	CMdUserApi(char *flowpath, char *servername, char *bid, char *iid, char *pd, char **InstrumentIDs, int InstrumentNum);
	virtual ~CMdUserApi(void);
	//13 api functions, come from MdApi
	void Init();
	void Join();
	const char *GetTradingDay();
	void RegisterFront(char *pszFrontAddress);
	void RegisterNameServer(char *pszNsAddress);
	void RegisterFensUserInfo(CThostFtdcFensUserInfoField *pFensUserInfo);
	void RegisterSpi();
	int SubscribeMarketData(char *ppInstrumentID[], int nCount);
	int UnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	//subforquote&unsubforquote is not supported by shanghaiCTP today-20140905.
	int SubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);
	int UnSubscribeForQuoteRsp(char *ppInstrumentID[], int nCount);
	int ReqUserLogin(void);
	int ReqUserLogout(void);

private:
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

	//检查是否出错
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//将出错消息送到消息队列
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//不送出错消息
	//根据OnFrontDisconnected(int nReason)的值填上错误消息
	void GetOnFrontDisconnectedMsg(CThostFtdcRspInfoField* pRspInfo);

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

	void input_DMDQ(CThostFtdcDepthMarketDataField *pDepthMarketData);
	CThostFtdcDepthMarketDataField *output_DMDQ(long *ts, long *tus, int *size);

private:
	CThostFtdcMdApi*			api;

	int	m_nRequestID;
	char *m_logFilePath;
	char *m_server;
	char *m_BrokerId;
	char *m_InvestorId;
	char *m_Password;

	char **m_InstrumentIDs;
	int m_InstrumentNum;

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
	
	CThostFtdcDepthMarketDataField *m_queue;
	long *m_intime_second;
	long *m_intime_usecond;
	int *m_current_size;
	int m_validmsg_size;
	int m_header_index;
	int m_tail_index;
	int m_queue_size;
};

#endif
