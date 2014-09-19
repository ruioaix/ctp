#include "traderapi.h"

#include <cstring>

using namespace std;

CTraderApi::CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *UserProductInfo, char *password, THOST_TE_RESUME_TYPE nResumeType)
{
	m_nRequestID = 0;
	m_logFilePath = flowpath;
	m_server = servername;
	m_BrokerId = brokerid;
	m_InvestorId = inverstorid;
	m_UserId = inverstorid;
	m_UserProductInfo = UserProductInfo;
	m_Password = password;
	api = CThostFtdcTraderApi::CreateFtdcTraderApi(m_logFilePath);
	if (api == NULL) {
		isError("tdapi created failed");
	}
	api->RegisterSpi(this);
	api->RegisterFront(m_server);
	api->SubscribePublicTopic(nResumeType);
	api->SubscribePrivateTopic(nResumeType);

	//verbose
	printtlb("create both spi and api object - td.");
	printtlc("save m_nRequestID: %d", m_nRequestID);
	printtlc("save m_logFilePath: %s", m_logFilePath);
	printtlc("save m_server: %s", m_server);
	printtlc("save m_BrokerId: %s", m_BrokerId);
	printtlc("save m_InvestorId: %s", m_InvestorId);
	printtlc("save m_Password: %s", m_Password);
}

CTraderApi::~CTraderApi(void) {
	api->RegisterSpi(NULL);
	api->Release();
	api=NULL;
	printtlb("free md");
}

void CTraderApi::Init(void) {
	api->Init();
	printtlb("api init");
}

const char *CTraderApi::GetTradingDay() {
	printtlb("GetTradingDay");
	return api->GetTradingDay();
}

void CTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {
	api->SubscribePublicTopic(nResumeType);
	printtlb("sub public topic, TYPE: %d", nResumeType);
}

void CTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {
	api->SubscribePrivateTopic(nResumeType);
	printtlb("sub private topic, TYPE: %d", nResumeType);
}

int CTraderApi::ReqUserLogin() {
	CThostFtdcReqUserLoginField request;
	memset(&request, 0, sizeof(CThostFtdcReqUserLoginField));
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcUserIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));

	printtlb("api req user login");
	printtlc("request.BrokerID: %s", request.BrokerID);
	printtlc("request.UserID: %s", request.UserID);

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


	CThostFtdcInputOrderField request; 
	memset(&request, 0, sizeof(CThostFtdcInputOrderField));

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

	printtlb("td api req order insert");
	return api->ReqOrderInsert(&request, ++m_nRequestID);
}

int CTraderApi::ReqQryInstrumentMarginRate(char *InstrumentID) {
	CThostFtdcQryInstrumentMarginRateField request;
	memset(&request, 0, sizeof(CThostFtdcQryInstrumentMarginRateField));
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.InstrumentID, InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	request.HedgeFlag = THOST_FTDC_HF_Speculation;

	printtlc("td request.BrokerID: %s", request.BrokerID);
	printtlc("td request.InvestorID: %s", request.InvestorID);
	printtlc("td request.InstrumentID: %s", request.InstrumentID);
	return api->ReqQryInstrumentMarginRate(&request, ++m_nRequestID);
}

int CTraderApi::ReqQryInstrument() {
	CThostFtdcQryInstrumentField request;
	memset(&request, 0 ,sizeof(CThostFtdcQryInstrumentField));
	//TODO
	return api->ReqQryInstrument(&request, ++m_nRequestID);
}

int CTraderApi::ReqSettlementInfoConfirm() {
	CThostFtdcSettlementInfoConfirmField request;
	memset(&request, 0, sizeof(CThostFtdcQrySettlementInfoConfirmField));
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));

	printtlb("ReqSettlementInfoConfirm called");
	return api->ReqSettlementInfoConfirm(&request, ++m_nRequestID);
}

int CTraderApi::ReqQrySettlementInfo() {
	CThostFtdcQrySettlementInfoField request;
	memset(&request, 0, sizeof(CThostFtdcQrySettlementInfoField));
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));

	printtlb("ReqQrySettlementInfo called");
	return api->ReqQrySettlementInfo(&request, ++m_nRequestID);
}

int CTraderApi::ReqAuthenticate() {
	CThostFtdcReqAuthenticateField request;
	memset(&request, 0, sizeof(CThostFtdcReqAuthenticateField));

	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_UserId, sizeof(TThostFtdcUserIDType));
	strncpy(request.UserProductInfo, m_UserProductInfo, sizeof(TThostFtdcProductInfoType));

	printtlb("ReqAuthenticate");
	return api->ReqAuthenticate(&request, ++m_nRequestID);
}

/********************************************************************************************************/
//call back 
/********************************************************************************************************/
void CTraderApi::OnFrontConnected() {
	printtlb("connected successfully.");

	ReqAuthenticate();
	//ReqUserLogin();

	//printtlb("request user login from here.");
}

void CTraderApi::OnFrontDisconnected(int nReason) {
	printtlb("connected failed, nReason: %d", nReason);
}

void CTraderApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//verbose
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
	//verbose
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
	//verbose
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
	//verbose
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
	//verbose
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
	//verbose
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

void CTraderApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//verbose
	printtlb("OnRspError called.");
	printtlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
}

void CTraderApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//verbose
	printtlb("OnRspAuthenticate called.");
	printtlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pRspAuthenticateField != NULL) {
		printtlc("BrokerID: %s, UserID: %s", pRspAuthenticateField->BrokerID, pRspAuthenticateField->UserID);
		printtlc("UserProductInfo: %s", pRspAuthenticateField->UserProductInfo);
	}
	else {
		printtlc("pRspAuthenticateField is NULL");
	}

	ReqUserLogin();
}
