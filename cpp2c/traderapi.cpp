#include "traderapi.h"

#include <cstring>

using namespace std;

/********************************************************************************************************/
CTraderApi::CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *userid, char *password, char *UserProductInfo, THOST_TE_RESUME_TYPE nResumeType) {

	m_nRequestID = 0;

	strncpy(m_logFilePath, flowpath, sizeof(m_logFilePath));
	strncpy(m_server, servername, sizeof(m_server));
	strncpy(m_BrokerId, brokerid, sizeof(TThostFtdcBrokerIDType));
	strncpy(m_InvestorId, inverstorid, sizeof(TThostFtdcInvestorIDType));
	strncpy(m_UserId, userid, sizeof(TThostFtdcUserIDType));
	strncpy(m_Password, password, sizeof(TThostFtdcPasswordType));
	strncpy(m_UserProductInfo, UserProductInfo, sizeof(TThostFtdcProductInfoType));

	api = CThostFtdcTraderApi::CreateFtdcTraderApi(m_logFilePath);
	if (api == NULL) {
		isError("tdapi created failed");
	}

	//verbose
	printtlb("create both spi and api object - td.");
	printtlc("save m_nRequestID: %d", m_nRequestID);
	printtlc("save m_logFilePath: %s", m_logFilePath);
	printtlc("save m_server: %s", m_server);
	printtlc("save m_BrokerId: %s", m_BrokerId);
	printtlc("save m_InvestorId: %s", m_InvestorId);
	printtlc("save m_UserId: %s", m_UserId);
	//printtlc("save m_Password: %s", m_Password);
	printtlc("save m_UserProductInfo: %s", m_UserProductInfo);

	api->RegisterSpi(this);
	api->RegisterFront(m_server);
	api->SubscribePublicTopic(nResumeType);
	api->SubscribePrivateTopic(nResumeType);
}
CTraderApi::~CTraderApi(void) {
	printtlb("free md");
	api->RegisterSpi(NULL);
	api->Release();
	api=NULL;
}

/********************************************************************************************************/
void CTraderApi::Init(void) {
	printtlb("api init");
	api->Init();
}
void CTraderApi::OnFrontConnected() {
	printtlb("connected successfully.");
	printtlb("request user login from here.");
	ReqUserLogin();
}
void CTraderApi::OnFrontDisconnected(int nReason) {
	printtlb("connected failed, nReason: %d", nReason);
}

/********************************************************************************************************/
const char *CTraderApi::GetTradingDay() {
	printtlb("GetTradingDay");
	return api->GetTradingDay();
}

/********************************************************************************************************/
void CTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printtlb("sub public topic, TYPE: %d", nResumeType);
	api->SubscribePublicTopic(nResumeType);
}
void CTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printtlb("sub private topic, TYPE: %d", nResumeType);
	api->SubscribePrivateTopic(nResumeType);
}

/**Request User Login************************************************************************************/
int CTraderApi::ReqUserLogin() {
	CThostFtdcReqUserLoginField request;
	memset(&request, 0, sizeof(CThostFtdcReqUserLoginField));

	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcUserIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));
	strncpy(request.UserProductInfo, m_UserProductInfo, sizeof(TThostFtdcProductInfoType));

	printtlb("api req user login");
	printtlc("request.BrokerID: %s", request.BrokerID);
	printtlc("request.UserID: %s", request.UserID);
	printtlc("request.UserProductInfo: %s", request.UserProductInfo);

	return api->ReqUserLogin(&request,++m_nRequestID);
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
		m_MaxOrderRef = strtol(pRspUserLogin->MaxOrderRef, NULL, 10);
		ReqSettlementInfoConfirm();
	}
	else {
		printtlc("td pRspUserLogin is NULL");
	}
}

/********************************************************************************************************/
int CTraderApi::ReqSettlementInfoConfirm() {
	CThostFtdcSettlementInfoConfirmField request;
	memset(&request, 0, sizeof(CThostFtdcQrySettlementInfoConfirmField));

	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));

	printtlb("ReqSettlementInfoConfirm called");
	return api->ReqSettlementInfoConfirm(&request, ++m_nRequestID);
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

/**Qry Settlement info***********************************************************************************/
int CTraderApi::ReqQrySettlementInfo() {
	CThostFtdcQrySettlementInfoField request;
	memset(&request, 0, sizeof(CThostFtdcQrySettlementInfoField));
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));

	printtlb("ReqQrySettlementInfo called");
	return api->ReqQrySettlementInfo(&request, ++m_nRequestID);
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

/********************************************************************************************************/
int CTraderApi::ReqOrderInsert(int OrderRef,\
	   	char *InstrumentID,\
		TThostFtdcDirectionType Direction,\
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
/*
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///组合开平标志
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///GTD日期
	TThostFtdcDateType	GTDDate;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	*/

	
	/**set automatically**************************************************************************************/
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.UserID, m_UserId, sizeof(TThostFtdcUserIDType));
	strncpy(request.InstrumentID, InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	//min volume: 1
	request.MinVolume = 1;
	//Force Close can only be done by CTPServerSide, here we can only set NotForceClose.
	request.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	//TODO we don't have Auto Suspend on clientside.
	request.IsAutoSuspend = 0;
	//TODO we don't have User Force Close on clientside.
	request.UserForceClose = 0;
	//TODO we don't use swap oder.
	request.IsSwapOrder = 0;

	/**set mannully******************************************************************************************/
	//buy or sell
	request.Direction = Direction;

	strncpy(request.CombOffsetFlag, CombOffsetFlag, sizeof(TThostFtdcCombOffsetFlagType));
	memcpy(request.CombHedgeFlag, CombHedgeFlag,sizeof(TThostFtdcCombHedgeFlagType));

	//volume type: 1=THOST_FTDC_VC_AV=any, 2=THOST_FTDC_VC_MV=min, 3=THOST_FTDC_VC_CV=all
	//order price type: 16kinds
	//time condition: 6kinds
	//volume total original: 
	//these five work together.
	request.VolumeCondition = VolumeCondition;
	request.OrderPriceType = OrderPriceType;
	request.LimitPrice = LimitPrice;
	request.TimeCondition = TimeCondition;
	request.ContingentCondition = ContingentCondition;
	request.StopPrice = StopPrice;
	request.VolumeTotalOriginal = VolumeTotalOriginal;

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

/********************************************************************************************************/
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

/********************************************************************************************************/
int CTraderApi::ReqQryInstrument() {
	CThostFtdcQryInstrumentField request;
	memset(&request, 0 ,sizeof(CThostFtdcQryInstrumentField));
	//TODO
	return api->ReqQryInstrument(&request, ++m_nRequestID);
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

/********************************************************************************************************/
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

/********************************************************************************************************/
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

/********************************************************************************************************/
void CTraderApi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	printtlb("td OnRtnOrder called");
}

void CTraderApi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	printtlb("td OnRtnTrade called");
}

