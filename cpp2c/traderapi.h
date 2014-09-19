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
	int ReqUserLogin();
	void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType);
	void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType);
	int ReqOrderInsert(int OrderRef, char *InstrumentID, TThostFtdcDirectionType Direction,\
		const TThostFtdcCombOffsetFlagType CombOffsetFlag,\
		const TThostFtdcCombHedgeFlagType CombHedgeFlag,\
		TThostFtdcVolumeType VolumeTotalOriginal,\
		TThostFtdcPriceType LimitPrice,\
		TThostFtdcOrderPriceTypeType OrderPriceType,\
		TThostFtdcTimeConditionType TimeCondition,\
		TThostFtdcContingentConditionType ContingentCondition,\
		TThostFtdcPriceType StopPrice,\
		TThostFtdcVolumeConditionType VolumeCondition);

private:
	//xx callback functions in CThostFtdcTraderSpi
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

private:
	CThostFtdcTraderApi *api;

	int	m_nRequestID;
	int m_MaxOrderRef;
	char *m_logFilePath;
	char *m_server;
	char *m_BrokerId;
	char *m_InvestorId;
	char *m_Password;
	char *m_UserProductInfo;
	char *m_AuthCode;
};

#endif //end of CTP_C_API_CPP2C_TRADERAPI_H
