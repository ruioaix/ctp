#ifndef CTP_C_API_CPP2C_TRADERAPI_H
#define CTP_C_API_CPP2C_TRADERAPI_H

#include "ctpcapi.h"
#include "cpp_api/ThostFtdcTraderApi.h"

using namespace std;

class CTraderApi : public CThostFtdcTraderSpi
{

public:
	CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, THOST_TE_RESUME_TYPE nResumeType);
	virtual ~CTraderApi(void);
	//xx api functions, from traderapi.
	void Init();
	void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType);
	void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType);
	int ReqUserLogin();
	int ReqSettlementInfoConfirm();

private:
	//xx callback functions in CThostFtdcTraderSpi
	virtual void OnFrontConnected();
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	CThostFtdcTraderApi *api;

	int	m_nRequestID;
	char *m_logFilePath;
	char *m_server;
	char *m_BrokerId;
	char *m_InvestorId;
	char *m_Password;
	char *m_UserProductInfo;
	char *m_AuthCode;
};

#endif //end of CTP_C_API_CPP2C_TRADERAPI_H
