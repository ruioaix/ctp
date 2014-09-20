#ifndef CTP_C_API_CPP2C_TRADERAPI_H
#define CTP_C_API_CPP2C_TRADERAPI_H

#include "ctpcapi.h"
#include "cpp_api/ThostFtdcTraderApi.h"

using namespace std;

class CTraderApi : public CThostFtdcTraderSpi
{

public:
	CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *userid, char *password, char *UserProductInfo, THOST_TE_RESUME_TYPE nResumeType);
	virtual ~CTraderApi(void);
	//xx api functions, from traderapi.
	void Init();
	const char *GetTradingDay();
	int ReqUserLogin();
	void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType);
	void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType);
	int ReqOrderInsert(\
			int OrderRef,\
		   	char *InstrumentID,\
			TThostFtdcDirectionType Direction,\
			TThostFtdcVolumeType VolumeTotalOriginal,\
			TThostFtdcPriceType LimitPrice,\
			TThostFtdcOrderPriceTypeType OrderPriceType,\
			TThostFtdcTimeConditionType TimeCondition,\
			TThostFtdcContingentConditionType ContingentCondition,\
			TThostFtdcPriceType StopPrice,\
			TThostFtdcVolumeConditionType VolumeCondition);
	int ReqOrderAction(CThostFtdcOrderField *pOrder, TThostFtdcActionFlagType ActionFlag);
	int ReqQryInstrumentMarginRate(char *InstrumentID);
	int ReqQryInstrument();
	int ReqSettlementInfoConfirm();
	int ReqQrySettlementInfo();

private:
	//xx callback functions in CThostFtdcTraderSpi
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	CThostFtdcTraderApi *api;

	int	m_nRequestID;
	int m_MaxOrderRef;
	char m_logFilePath[1000];
	char m_server[1000];
	TThostFtdcBrokerIDType m_BrokerId;
	TThostFtdcUserIDType m_UserId;
	TThostFtdcInvestorIDType m_InvestorId;
	TThostFtdcPasswordType m_Password;
	TThostFtdcProductInfoType m_UserProductInfo;
};

#endif //end of CTP_C_API_CPP2C_TRADERAPI_H
