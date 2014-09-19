#include "traderapi.h"

#include <cstring>

using namespace std;

CTraderApi::CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, THOST_TE_RESUME_TYPE nResumeType)
{
	printtlb("create both spi and api object - td.");
	m_nRequestID = 0;
	printtlc("save m_nRequestID: %d", m_nRequestID);
	m_logFilePath = flowpath;
	printtlc("save m_logFilePath: %s", m_logFilePath);
	m_server = servername;
	printtlc("save m_server: %s", m_server);
	m_BrokerId = brokerid;
	printtlc("save m_BrokerId: %s", m_BrokerId);
	m_InvestorId = inverstorid;
	printtlc("save m_InvestorId: %s", m_InvestorId);
	m_Password = password;
	printtlc("save m_Password: %s", m_Password);

	api = CThostFtdcTraderApi::CreateFtdcTraderApi(m_logFilePath);
	if (api == NULL) {
		isError("tdapi created failed");
	}
	api->RegisterSpi(this);
	api->RegisterFront(m_server);
	api->SubscribePublicTopic(nResumeType);
	api->SubscribePrivateTopic(nResumeType);
}

CTraderApi::~CTraderApi(void) {
	api->RegisterSpi(NULL);
	api->Release();
	api=NULL;
	printtlb("delete md");
}

void CTraderApi::Init(void) {
	printtlb("td api init");
	api->Init();
}

void CTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printtlb("td api sub public topic, TYPE: %d", nResumeType);
	api->SubscribePublicTopic(nResumeType);
}

void CTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printtlb("td api sub private topic, TYPE: %d", nResumeType);
	api->SubscribePrivateTopic(nResumeType);
}

int CTraderApi::ReqUserLogin() {
	printtlb("td api req user login");

	CThostFtdcReqUserLoginField request = {};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));
	printtlc("td request.BrokerID: %s", request.BrokerID);
	printtlc("td request.UserID: %s", request.UserID);
	printtlc("td request.Password: %s", request.Password);

	return api->ReqUserLogin(&request,++m_nRequestID);
}

int CTraderApi::ReqOrderInsert(int OrderRef, char *InstrumentID, TThostFtdcDirectionType Direction,\
		const TThostFtdcCombOffsetFlagType CombOffsetFlag,\
		const TThostFtdcCombHedgeFlagType CombHedgeFlag,\
		TThostFtdcVolumeType VolumeTotalOriginal,\
		TThostFtdcPriceType LimitPrice,\
		TThostFtdcOrderPriceTypeType OrderPriceType,\
		TThostFtdcTimeConditionType TimeCondition,\
		TThostFtdcContingentConditionType ContingentCondition,\
		TThostFtdcPriceType StopPrice,\
		TThostFtdcVolumeConditionType VolumeCondition) {

	printtlb("td api req order insert");

	CThostFtdcInputOrderField request = {}; 

	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));

	request.MinVolume = 1;
	request.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	request.IsAutoSuspend = 0;
	request.UserForceClose = 0;
	request.IsSwapOrder = 0;

	strncpy(request.InstrumentID, InstrumentID, sizeof(TThostFtdcInstrumentIDType));

	request.Direction = Direction;

	strncpy(request.CombOffsetFlag, CombOffsetFlag, sizeof(TThostFtdcCombOffsetFlagType));

	request.OrderPriceType = OrderPriceType;
	request.LimitPrice = LimitPrice;

	request.VolumeTotalOriginal = VolumeTotalOriginal;

	memcpy(request.CombHedgeFlag, CombHedgeFlag,sizeof(TThostFtdcCombHedgeFlagType));

	request.VolumeCondition = VolumeCondition;
	request.TimeCondition = TimeCondition;
	request.ContingentCondition = ContingentCondition;
	request.StopPrice = StopPrice;
	if (OrderRef < 0) {
		int nRet = m_MaxOrderRef;
		sprintf(request.OrderRef, "%d", nRet);
		++m_MaxOrderRef;
	}
	else {
		sprintf(request.OrderRef, "%d", OrderRef);
	}

	int rt = api->ReqOrderInsert(&request, ++m_nRequestID);
	return rt;
}

int CTraderApi::ReqQryInstrumentMarginRate(char *InstrumentID) {

	CThostFtdcQryInstrumentMarginRateField request = {};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.InstrumentID, InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	printtlc("td request.BrokerID: %s", request.BrokerID);
	printtlc("td request.InvestorID: %s", request.InvestorID);
	printtlc("td request.InstrumentID: %s", request.InstrumentID);
	request.HedgeFlag = THOST_FTDC_HF_Speculation;
	return api->ReqQryInstrumentMarginRate(&request, ++m_nRequestID);
}

int CTraderApi::ReqQryInstrument() {
	CThostFtdcQryInstrumentField request;
	memset(&request, 0 ,sizeof(CThostFtdcQryInstrumentField));
	return api->ReqQryInstrument(&request, ++m_nRequestID);
}

int CTraderApi::ReqSettlementInfoConfirm() {
	printtlb("ReqSettlementInfoConfirm called");
	CThostFtdcSettlementInfoConfirmField request = {};

	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	//TThostFtdcDateType	ConfirmDate;
	//TThostFtdcTimeType	ConfirmTime;
	return api->ReqSettlementInfoConfirm(&request, ++m_nRequestID);
}

int CTraderApi::ReqQrySettlementInfo() {
	printtlb("ReqQrySettlementInfo called");
	CThostFtdcQrySettlementInfoField request = {};

	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	return api->ReqQrySettlementInfo(&request, ++m_nRequestID);
}
/********************************************************************************************************/
//call back 
/********************************************************************************************************/
void CTraderApi::OnFrontConnected() {
	printtlb("td connected successfully.");
	printtlb("td request user login from here.");
	ReqUserLogin();
}

void CTraderApi::OnFrontDisconnected(int nReason) {
	printtlb("td connected failed, nReason: %d", nReason);
}

void CTraderApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printtlb("td OnRspUserLogin called.");
	printtlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pRspUserLogin != NULL) {
		printtlc("td pRspUserLogin->TradingDay: %s", pRspUserLogin->TradingDay);
		printtlc("td pRspUserLogin->LoginTime: %s", pRspUserLogin->LoginTime);
		printtlc("td pRspUserLogin->BrokerID: %s", pRspUserLogin->BrokerID);
		printtlc("td pRspUserLogin->UserID: %s", pRspUserLogin->UserID); 
		printtlc("td pRspUserLogin->SystemName: %s", pRspUserLogin->SystemName);
		printtlc("td pRspUserLogin->FrontID: %d", pRspUserLogin->FrontID);
		printtlc("td pRspUserLogin->SessionID: %d", pRspUserLogin->SessionID);
		printtlc("td pRspUserLogin->MaxOrderRef: %s", pRspUserLogin->MaxOrderRef);
		printtlc("td pRspUserLogin->SHFETime: %s", pRspUserLogin->SHFETime); 
		printtlc("td pRspUserLogin->DCETime: %s", pRspUserLogin->DCETime);
		printtlc("td pRspUserLogin->CZCETime: %s", pRspUserLogin->CZCETime);
		printtlc("td pRspUserLogin->FFEXTime: %s", pRspUserLogin->FFEXTime);
		printtlc("td pRspUserLogin->INETime: %s", pRspUserLogin->INETime);
		printtlb("td reqsettlement from here");
		ReqSettlementInfoConfirm();
	}
	else {
		printtlc("td pRspUserLogin is NULL");
	}
}

void CTraderApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printtlb("td OnRspOrderInsert called.");
	printtlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pInputOrder != NULL) {
	}
	else {
		printtlc("td pInputOrder is NULL");
	}
}

void CTraderApi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	printtlb("td OnRtnOrder called");
}

void CTraderApi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	printtlb("td OnRtnTrade called");
}

void CTraderApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printtlb("td OnRspQryInstrumentCommissionRate called.");
	printtlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pInstrumentCommissionRate != NULL) {
		printtlb("InstrumentID: %s", pInstrumentCommissionRate->InstrumentID);
		printtlb("InvestorRange: %d", pInstrumentCommissionRate->InvestorRange);
		printtlb("BrokerID: %s, InvestorID: %s", pInstrumentCommissionRate->BrokerID, pInstrumentCommissionRate->InvestorID);
		printtlb("OpenRatioByMoney: %f", pInstrumentCommissionRate->OpenRatioByMoney);
		printtlb("OpenRatioByVolume: %f", pInstrumentCommissionRate->OpenRatioByVolume);
		printtlb("CloseRatioByMoney: %f", pInstrumentCommissionRate->CloseRatioByMoney);
		printtlb("CloseRatioByVolume: %f", pInstrumentCommissionRate->CloseRatioByVolume);
		printtlb("CloseTodayRatioByMoney: %f", pInstrumentCommissionRate->CloseTodayRatioByMoney);
		printtlb("CloseTodayRatioByVolume: %f", pInstrumentCommissionRate->CloseTodayRatioByVolume);
	}
	else {
		printtlc("td pInstrumentCommissionRate is NULL");
	}
}

void CTraderApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printtlb("td OnRspSettlementInfoConfirm called.");
	printtlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pSettlementInfoConfirm != NULL) {
		printtlb("td BrokerID: %s, InvestorID: %s", pSettlementInfoConfirm->BrokerID, pSettlementInfoConfirm->InvestorID);
	}
	else {
		printtlc("td pSettlementInfoConfirm is NULL");
	}
}

void CTraderApi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printtlb("td OnRspQrySettlementInfo called.");
	printtlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pSettlementInfo != NULL) {
		printtlb("td BrokerID: %s, InvestorID: %s", pSettlementInfo->BrokerID, pSettlementInfo->InvestorID);
	}
	else {
		printtlc("td pSettlementInfoConfirm is NULL");
	}
}

void CTraderApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printtlb("td OnRspQryInstrument called.");
	printtlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pInstrument != NULL) {
		printtlb("td InstrumentID: %s, InstrumentName: %s", pInstrument->InstrumentID, pInstrument->InstrumentName);
	}
	else {
		printtlc("td pSettlementInfoConfirm is NULL");
	}
}

