#include "traderapi.h"

#include <cstring>

using namespace std;

/********************************************************************************************************/
CTraderApi::CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *userid, char *password, char *UserProductInfo, THOST_TE_RESUME_TYPE nResumeType) {

	m_nRequestID = 0;
	ready = 0;

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
int CTraderApi::IsReady() {
	return ready;
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
int CTraderApi::ReqOrderInsert(int OrderRef,\
	   	char *InstrumentID,\
		TThostFtdcDirectionType Direction,\
		int CombOffsetFlag,\
		TThostFtdcVolumeType VolumeTotalOriginal,\
		TThostFtdcPriceType LimitPrice,\
		TThostFtdcOrderPriceTypeType OrderPriceType,\
		TThostFtdcTimeConditionType TimeCondition,\
		TThostFtdcContingentConditionType ContingentCondition,\
		TThostFtdcPriceType StopPrice,\
		TThostFtdcVolumeConditionType VolumeCondition) {

	CThostFtdcInputOrderField request; 
	memset(&request, 0, sizeof(CThostFtdcInputOrderField));
	
	/**set automatically**************************************************************************************/
	memcpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	memcpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	memcpy(request.UserID, m_UserId, sizeof(TThostFtdcUserIDType));
	memcpy(request.InstrumentID, InstrumentID, sizeof(TThostFtdcInstrumentIDType));
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
	//TODO we don't use GTDDate & BusinessUnit & RequestID.
	//request.GTDDate, request.BusinessUnit, request.RequestID is initial with zero.
	//TODO now we only touji.
	strcpy(request.CombHedgeFlag, "1");

	/**set mannully******************************************************************************************/
	//buy or sell
	request.Direction = Direction;
	
	sprintf(request.CombOffsetFlag, "%d", CombOffsetFlag);

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

	//TODO OrderRef need syn
	if (OrderRef < 0) {
		int nRet = m_MaxOrderRef;
		sprintf(request.OrderRef, "%d", nRet);
		++m_MaxOrderRef;
	}
	else {
		sprintf(request.OrderRef, "%d", OrderRef);
	}

	printtlb("api reqorder insert");
	printtlc("BrokerID: %s", request.BrokerID);
	printtlc("InvestorID: %s", request.InvestorID);
	printtlc("InstrumentID: %s", request.InstrumentID);
	printtlc("OrderRef: %s", request.OrderRef);
	printtlc("UserID: %s", request.UserID);
	printtlc("OrderPriceType: %c", request.OrderPriceType);
	printtlc("Direction: %c", request.Direction);
	printtlc("CombOffsetFlag: %s", request.CombOffsetFlag);
	printtlc("CombHedgeFlag: %s", request.CombHedgeFlag);
	printtlc("LimitPrice: %f", request.LimitPrice);
	printtlc("VolumeTotalOriginal: %d", request.VolumeTotalOriginal);
	printtlc("TimeCondition: %c", request.TimeCondition);
	printtlc("GTDDate: %s", request.GTDDate);
	printtlc("VolumeCondition: %c", request.VolumeCondition);
	printtlc("MinVolume: %d", request.MinVolume);
	printtlc("ContingentCondition: %c", request.ContingentCondition);
	printtlc("StopPrice: %f", request.StopPrice);
	printtlc("ForceCloseReason: %c", request.ForceCloseReason);
	printtlc("IsAutoSuspend: %d", request.IsAutoSuspend);
	printtlc("BusinessUnit: %s", request.BusinessUnit);
	printtlc("RequestID: %d", request.RequestID);
	printtlc("UserForceClose: %d", request.UserForceClose);
	printtlc("IsSwapOrder: %d", request.IsSwapOrder);
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
		printtlc("BrokerID: %s", pInputOrder->BrokerID);
		printtlc("InvestorID: %s", pInputOrder->InvestorID);
		printtlc("InstrumentID: %s", pInputOrder->InstrumentID);
		printtlc("OrderRef: %s", pInputOrder->OrderRef);
		printtlc("UserID: %s", pInputOrder->UserID);
		printtlc("OrderPriceType: %c", pInputOrder->OrderPriceType);
		printtlc("Direction: %c", pInputOrder->Direction);
		printtlc("CombOffsetFlag: %s", pInputOrder->CombOffsetFlag);
		printtlc("CombHedgeFlag: %s", pInputOrder->CombHedgeFlag);
		printtlc("LimitPrice: %f", pInputOrder->LimitPrice);
		printtlc("VolumeTotalOriginal: %d", pInputOrder->VolumeTotalOriginal);
		printtlc("TimeCondition: %c", pInputOrder->TimeCondition);
		printtlc("GTDDate: %s", pInputOrder->GTDDate);
		printtlc("VolumeCondition: %c", pInputOrder->VolumeCondition);
		printtlc("MinVolume: %d", pInputOrder->MinVolume);
		printtlc("ContingentCondition: %c", pInputOrder->ContingentCondition);
		printtlc("StopPrice: %f", pInputOrder->StopPrice);
		printtlc("ForceCloseReason: %c", pInputOrder->ForceCloseReason);
		printtlc("IsAutoSuspend: %d", pInputOrder->IsAutoSuspend);
		printtlc("BusinessUnit: %s", pInputOrder->BusinessUnit);
		printtlc("RequestID: %d", pInputOrder->RequestID);
		printtlc("UserForceClose: %d", pInputOrder->UserForceClose);
		printtlc("IsSwapOrder: %d", pInputOrder->IsSwapOrder);
	}
	else {
		printtlc("td pInputOrder is NULL");
	}
}
//when order is accepted by ctp server
void CTraderApi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	printtlb("td OnRtnOrder called");
	printtlc("BrokerID: %s", pOrder->BrokerID);
	printtlc("InvestorID: %s", pOrder->InvestorID);
	printtlc("InstrumentID: %s", pOrder->InstrumentID);
	printtlc("OrderRef: %s", pOrder->OrderRef);
	printtlc("UserID: %s", pOrder->UserID);
	printtlc("OrderPriceType: %c", pOrder->OrderPriceType);
	printtlc("Direction: %c", pOrder->Direction);
	printtlc("CombOffsetFlag: %s", pOrder->CombOffsetFlag);
	printtlc("CombHedgeFlag: %s", pOrder->CombHedgeFlag);
	printtlc("LimitPrice: %f", pOrder->LimitPrice);
	printtlc("VolumeTotalOriginal: %d", pOrder->VolumeTotalOriginal);
	printtlc("TimeCondition: %c", pOrder->TimeCondition);
	printtlc("GTDDate: %s", pOrder->GTDDate);
	printtlc("VolumeCondition: %c", pOrder->VolumeCondition);
	printtlc("MinVolume: %d", pOrder->MinVolume);
	printtlc("ContingentCondition: %c", pOrder->ContingentCondition);
	printtlc("StopPrice: %f", pOrder->StopPrice);
	printtlc("ForceCloseReason: %c", pOrder->ForceCloseReason);
	printtlc("IsAutoSuspend: %d", pOrder->IsAutoSuspend);
	printtlc("BusinessUnit: %s", pOrder->BusinessUnit);
	printtlc("RequestID: %d", pOrder->RequestID);
	printtlc("OrderLocalID: %s", pOrder->OrderLocalID);
	printtlc("ExchangeID: %s", pOrder->ExchangeID);
	printtlc("ParticipantID: %s", pOrder->ParticipantID);
	printtlc("ClientID: %s", pOrder->ClientID);
	printtlc("ExchangeInstID: %s", pOrder->ExchangeInstID);
	printtlc("TraderID: %s", pOrder->TraderID);
	printtlc("InstallID: %d", pOrder->InstallID);
	printtlc("OrderSubmitStatus: %d", pOrder->OrderSubmitStatus);
	printtlc("NotifySequence: %d", pOrder->NotifySequence);
	printtlc("TradingDay: %s", pOrder->TradingDay);
	printtlc("SettlementID: %d", pOrder->SettlementID);
	printtlc("OrderSysID: %s", pOrder->OrderSysID);
	printtlc("OrderSource: %c", pOrder->OrderSource);
	printtlc("OrderStatus: %c", pOrder->OrderStatus);
	printtlc("OrderType: %c", pOrder->OrderType);
	printtlc("VolumeTraded: %d", pOrder->VolumeTraded);
	printtlc("VolumeTotal: %d", pOrder->VolumeTotal);
	printtlc("InsertDate: %s", pOrder->InsertDate);
	printtlc("InsertTime: %s", pOrder->InsertTime);
	printtlc("ActiveTime: %s", pOrder->ActiveTime);
	printtlc("SuspendTime: %s", pOrder->SuspendTime);
	printtlc("UpdateTime: %s", pOrder->UpdateTime);
	printtlc("CancelTime: %s", pOrder->CancelTime);
	printtlc("ActiveTraderID: %s", pOrder->ActiveTraderID);
	printtlc("ClearingPartID: %s", pOrder->ClearingPartID);
	printtlc("SequenceNo: %d", pOrder->SequenceNo);
	printtlc("FrontID: %d", pOrder->FrontID);
	printtlc("SessionID: %d", pOrder->SessionID);
	printtlc("UserProductInfo: %s", pOrder->UserProductInfo);
	printtlc("StatusMsg: %s", pOrder->StatusMsg);
	printtlc("UserForceClose: %d", pOrder->UserForceClose);
	printtlc("ActiveUserID: %s", pOrder->ActiveUserID);
	printtlc("BrokerOrderSeq: %d", pOrder->BrokerOrderSeq);
	printtlc("RelativeOrderSysID: %s", pOrder->RelativeOrderSysID);
	printtlc("ZCETotalTradedVolume: %d", pOrder->ZCETotalTradedVolume);
	printtlc("IsSwapOrder: %d", pOrder->IsSwapOrder);
}
void CTraderApi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	printtlb("td OnRtnTrade called");
	printtlc("BrokerID: %s", pTrade->BrokerID);
	printtlc("InvestorID: %s", pTrade->InvestorID);
	printtlc("InstrumentID: %s", pTrade->InstrumentID);
	printtlc("OrderRef: %s", pTrade->OrderRef);
	printtlc("UserID: %s", pTrade->UserID);
	printtlc("ExchangeID: %s", pTrade->ExchangeID);
	printtlc("TradeID: %s", pTrade->TradeID);
	printtlc("Direction: %c", pTrade->Direction);
	printtlc("OrderSysID: %s", pTrade->OrderSysID);
	printtlc("ParticipantID: %s", pTrade->ParticipantID);
	printtlc("ClientID: %s", pTrade->ClientID);
	printtlc("TradingRole: %c", pTrade->TradingRole);
	printtlc("ExchangeInstID: %s", pTrade->ExchangeInstID);
	printtlc("OffsetFlag: %c", pTrade->OffsetFlag);
	printtlc("HedgeFlag: %c", pTrade->HedgeFlag);
	printtlc("Price: %f", pTrade->Price);
	printtlc("Volume: %d", pTrade->Volume);
	printtlc("TradeDate: %s", pTrade->TradeDate);
	printtlc("TradeTime: %s", pTrade->TradeTime);
	printtlc("TradeType: %c", pTrade->TradeType);
	printtlc("PriceSource: %c", pTrade->PriceSource);
	printtlc("TraderID: %s", pTrade->TraderID);
	printtlc("OrderLocalID: %s", pTrade->OrderLocalID);
	printtlc("ClearingPartID: %s", pTrade->ClearingPartID);
	printtlc("BusinessUnit: %s", pTrade->BusinessUnit);
	printtlc("SequenceNo: %d", pTrade->SequenceNo);
	printtlc("TradingDay: %s", pTrade->TradingDay);
	printtlc("SettlementID: %d", pTrade->SettlementID);
	printtlc("BrokerOrderSeq: %d", pTrade->BrokerOrderSeq);
	printtlc("TradeSource: %c", pTrade->TradeSource);
}
void CTraderApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) {
	//verbose
	printtlb("td OnErrRtnOrderInsert called.");
	if (pRspInfo != NULL) {
		printtlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printtlc("td pRspInfo is NULL");
	}
	if (pInputOrder != NULL) {
		printtlc("BrokerID: %s", pInputOrder->BrokerID);
		printtlc("InvestorID: %s", pInputOrder->InvestorID);
		printtlc("InstrumentID: %s", pInputOrder->InstrumentID);
		printtlc("OrderRef: %s", pInputOrder->OrderRef);
		printtlc("UserID: %s", pInputOrder->UserID);
		printtlc("OrderPriceType: %c", pInputOrder->OrderPriceType);
		printtlc("Direction: %c", pInputOrder->Direction);
		printtlc("CombOffsetFlag: %s", pInputOrder->CombOffsetFlag);
		printtlc("CombHedgeFlag: %s", pInputOrder->CombHedgeFlag);
		printtlc("LimitPrice: %f", pInputOrder->LimitPrice);
		printtlc("VolumeTotalOriginal: %d", pInputOrder->VolumeTotalOriginal);
		printtlc("TimeCondition: %c", pInputOrder->TimeCondition);
		printtlc("GTDDate: %s", pInputOrder->GTDDate);
		printtlc("VolumeCondition: %c", pInputOrder->VolumeCondition);
		printtlc("MinVolume: %d", pInputOrder->MinVolume);
		printtlc("ContingentCondition: %c", pInputOrder->ContingentCondition);
		printtlc("StopPrice: %f", pInputOrder->StopPrice);
		printtlc("ForceCloseReason: %c", pInputOrder->ForceCloseReason);
		printtlc("IsAutoSuspend: %d", pInputOrder->IsAutoSuspend);
		printtlc("BusinessUnit: %s", pInputOrder->BusinessUnit);
		printtlc("RequestID: %d", pInputOrder->RequestID);
		printtlc("UserForceClose: %d", pInputOrder->UserForceClose);
		printtlc("IsSwapOrder: %d", pInputOrder->IsSwapOrder);
	}
	else {
		printtlc("td pInputOrder is NULL");
	}
}

/********************************************************************************************************/
int CTraderApi::ReqOrderAction(CThostFtdcOrderField *pOrder, TThostFtdcActionFlagType ActionFlag) {
	CThostFtdcInputOrderActionField request;
	memset(&request, 0, sizeof(CThostFtdcInputOrderActionField));

	/**set automatically**************************************************************************************/
	memcpy(request.BrokerID, pOrder->BrokerID, sizeof(TThostFtdcBrokerIDType));
	memcpy(request.InvestorID, pOrder->InvestorID, sizeof(TThostFtdcInvestorIDType));
	memcpy(request.UserID, pOrder->UserID, sizeof(TThostFtdcUserIDType));
	memcpy(request.InstrumentID, pOrder->InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	memcpy(request.OrderRef, pOrder->OrderRef, sizeof(TThostFtdcOrderRefType));
	memcpy(request.ExchangeID, pOrder->ExchangeID, sizeof(TThostFtdcExchangeIDType));
	memcpy(request.OrderSysID, pOrder->OrderSysID, sizeof(TThostFtdcOrderSysIDType));

	request.ActionFlag = ActionFlag;
	request.FrontID = pOrder->FrontID;
	request.SessionID = pOrder->SessionID;

	//TODO
	//TThostFtdcOrderActionRefType	OrderActionRef;
	//TThostFtdcRequestIDType	RequestID;
	//TThostFtdcPriceType	LimitPrice;
	//TThostFtdcVolumeType	VolumeChange;
	return api->ReqOrderAction(&request, ++m_nRequestID);
}
void CTraderApi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//TODO
}
void CTraderApi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) {
	//TODO
}

/********************************************************************************************************/
int CTraderApi::ReqQueryMaxOrderVolume(char *InstrumentID, TThostFtdcDirectionType Direction, TThostFtdcOffsetFlagType OffsetFlag, TThostFtdcHedgeFlagType HedgeFlag) {
	CThostFtdcQueryMaxOrderVolumeField request;
	memset(&request, 0, sizeof(CThostFtdcQueryMaxOrderVolumeField));

	memcpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	memcpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	memcpy(request.InstrumentID, InstrumentID, sizeof(TThostFtdcInstrumentIDType));
	request.Direction = Direction;
	request.OffsetFlag = OffsetFlag;
	request.HedgeFlag = HedgeFlag;
	//TODO
	//TThostFtdcVolumeType	MaxVolume;
	return api->ReqQueryMaxOrderVolume(&request, ++m_nRequestID);
}
void CTraderApi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	//TODO
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
		ready=1;
	}
	else {
		printtlc("td pSettlementInfoConfirm is NULL");
	}
}

/********************************************************************************************************/
int CTraderApi::ReqQryTradingAccount() {
	CThostFtdcQryTradingAccountField request;
	memset(&request, 0, sizeof(CThostFtdcQryTradingAccountField));

	memcpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	memcpy(request.InvestorID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.CurrencyID, "CNY", sizeof(TThostFtdcCurrencyIDType));

	return api->ReqQryTradingAccount(&request, ++m_nRequestID);
}
void CTraderApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printtlb("OnRspQryTradingAccount called");
	printtlc("BrokerID: %s", pTradingAccount->BrokerID);
	printtlc("AccountID: %s", pTradingAccount->AccountID);
	printtlc("PreMortgage: %f", pTradingAccount->PreMortgage);
	printtlc("PreCredit: %f", pTradingAccount->PreCredit);
	printtlc("PreDeposit: %f", pTradingAccount->PreDeposit);
	printtlc("PreBalance: %f", pTradingAccount->PreBalance);
	printtlc("PreMargin: %f", pTradingAccount->PreMargin);
	printtlc("InterestBase: %f", pTradingAccount->InterestBase);
	printtlc("Interest: %f", pTradingAccount->Interest);
	printtlc("Deposit: %f", pTradingAccount->Deposit);
	printtlc("Withdraw: %f", pTradingAccount->Withdraw);
	printtlc("FrozenMargin: %f", pTradingAccount->FrozenMargin);
	printtlc("FrozenCash: %f", pTradingAccount->FrozenCash);
	printtlc("FrozenCommission: %f", pTradingAccount->FrozenCommission);
	printtlc("CurrMargin: %f", pTradingAccount->CurrMargin);
	printtlc("CashIn: %f", pTradingAccount->CashIn);
	printtlc("Commission: %f", pTradingAccount->Commission);
	printtlc("CloseProfit: %f", pTradingAccount->CloseProfit);
	printtlc("PositionProfit: %f", pTradingAccount->PositionProfit);
	printtlc("Balance: %f", pTradingAccount->Balance);
	printtlc("Available: %f", pTradingAccount->Available);
	printtlc("WithdrawQuota: %f", pTradingAccount->WithdrawQuota);
	printtlc("Reserve: %f", pTradingAccount->Reserve);
	printtlc("TradingDay: %s", pTradingAccount->TradingDay);
	printtlc("SettlementID: %d", pTradingAccount->SettlementID);
	printtlc("Credit: %f", pTradingAccount->Credit);
	printtlc("Mortgage: %f", pTradingAccount->Mortgage);
	printtlc("ExchangeMargin: %f", pTradingAccount->ExchangeMargin);
	printtlc("DeliveryMargin: %f", pTradingAccount->DeliveryMargin);
	printtlc("ExchangeDeliveryMargin: %f", pTradingAccount->ExchangeDeliveryMargin);
	printtlc("ReserveBalance: %f", pTradingAccount->ReserveBalance);
	printtlc("CurrencyID: %s", pTradingAccount->CurrencyID);
	printtlc("PreFundMortgageIn: %f", pTradingAccount->PreFundMortgageIn);
	printtlc("PreFundMortgageOut: %f", pTradingAccount->PreFundMortgageOut);
	printtlc("FundMortgageIn: %f", pTradingAccount->FundMortgageIn);
	printtlc("FundMortgageOut: %f", pTradingAccount->FundMortgageOut);
	printtlc("FundMortgageAvailable: %f", pTradingAccount->FundMortgageAvailable);
	printtlc("MortgageableFund: %f", pTradingAccount->MortgageableFund);
	printtlc("SpecProductMargin: %f", pTradingAccount->SpecProductMargin);
	printtlc("SpecProductFrozenMargin: %f", pTradingAccount->SpecProductFrozenMargin);
	printtlc("SpecProductCommission: %f", pTradingAccount->SpecProductCommission);
	printtlc("SpecProductFrozenCommission: %f", pTradingAccount->SpecProductFrozenCommission);
	printtlc("SpecProductPositionProfit: %f", pTradingAccount->SpecProductPositionProfit);
	printtlc("SpecProductCloseProfit: %f", pTradingAccount->SpecProductCloseProfit);
	printtlc("SpecProductPositionProfitByAlg: %f", pTradingAccount->SpecProductPositionProfitByAlg);
	printtlc("SpecProductExchangeMargin: %f", pTradingAccount->SpecProductExchangeMargin);
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
