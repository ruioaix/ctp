#ifndef CTP_C_API_CPP2C_MDUSERAPI_H
#define CTP_C_API_CPP2C_MDUSERAPI_H

#include "ctpcapi.h"
#include "cpp_api/ThostFtdcMdApi.h"

using namespace std;

class CMdUserApi : public CThostFtdcMdSpi
{

public:
	virtual ~CMdUserApi(void);
	//there are 13 functions in CThostFtdcMdApi. (ignore deconstruct function)
	//md object. 2 api functions: CreateFtdcMdApi & Release are included in the following two.
	CMdUserApi(char *flowpath, char *servername, char *bid, char *iid, char *pd, char **InstrumentIDs, int InstrumentNum);
	//11 api functions, come from MdApi
	void Init();
	void Join();
	const char *GetTradingDay();
	void RegisterFront(char *pszFrontAddress);
	void RegisterNameServer(char *pszNsAddress);
	void RegisterFensUserInfo();
	void RegisterSpi();
	int SubscribeMarketData(char *ppInstrumentID[], int nCount);
	int UnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	int ReqUserLogin(void);
	int ReqUserLogout(void);

private:
	//9 callback functions in CThostFtdcMdSpi
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
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

public:
	//9 callback register functions 
	void RegisterCallback_ofc(fnOnFrontConnected pCallback) { m_fnOnFrontConnected = pCallback; }
	void RegisterCallback_ofd(fnOnFrontDisconnected pCallback) { m_fnOnFrontDisconnected = pCallback; }
	void RegisterCallback_ohb(fnOnHeartBeatWarning pCallback) { m_fnOnHeartBeatWarning = pCallback; }
	void RegisterCallback_oruli(fnOnRspUserLogin pCallback) { m_fnOnRspUserLogin = pCallback; }
	void RegisterCallback_orulo(fnOnRspUserLogout pCallback) { m_fnOnRspUserLogout = pCallback; }
	void RegisterCallback_ore(fnOnRspError pCallback) { m_fnOnRspError = pCallback; }
	void RegisterCallback_orsmd(fnOnRspSubMarketData pCallback) { m_fnOnRspSubMarketData = pCallback; }
	void RegisterCallback_orusmd(fnOnRspUnSubMarketData pCallback) { m_fnOnRspUnSubMarketData = pCallback; }
	void RegisterCallback_ordmd(fnOnRtnDepthMarketData pCallback) { m_fnOnRtnDepthMarketData = pCallback; }

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
	fnOnRtnDepthMarketData m_fnOnRtnDepthMarketData;
	
	CThostFtdcDepthMarketDataField *m_queue;
	long *m_intime_second;
	long *m_intime_usecond;
	int *m_current_size;
	int m_validmsg_size;
	int m_header_index;
	int m_tail_index;
	int m_queue_size;
};

#endif //end of CTP_C_API_CPP2C_MDUSERAPI_H
