#include "traderapi.h"
#include "base.h"

#include <cstring>

using namespace std;

CTraderApi::CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, THOST_TE_RESUME_TYPE nResumeType)
{
	printlb("create both spi and api object - td.");
	m_nRequestID = 0;
	printlc("save m_nRequestID: %d", m_nRequestID);
	m_logFilePath = flowpath;
	printlc("save m_logFilePath: %s", m_logFilePath);
	m_server = servername;
	printlc("save m_server: %s", m_server);
	m_BrokerId = brokerid;
	printlc("save m_BrokerId: %s", m_BrokerId);
	m_InvestorId = inverstorid;
	printlc("save m_InvestorId: %s", m_InvestorId);
	m_Password = password;
	printlc("save m_Password: %s", m_Password);

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
	printlb("delete md");
}

void CTraderApi::Init(void) {
	printlb("td api init");
	api->Init();
}

void CTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printlb("td api sub public topic, TYPE: %d", nResumeType);
	api->SubscribePublicTopic(nResumeType);
}

void CTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printlb("td api sub private topic, TYPE: %d", nResumeType);
	api->SubscribePrivateTopic(nResumeType);
}

int CTraderApi::ReqUserLogin() {
	printlb("td api req user login");

	CThostFtdcReqUserLoginField request = {};
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));
	printlc("td request.BrokerID: %s", request.BrokerID);
	printlc("td request.UserID: %s", request.UserID);
	printlc("td request.Password: %s", request.Password);

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

	printlb("td api req order insert");

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
	printlc("td request.BrokerID: %s", request.BrokerID);
	printlc("td request.InvestorID: %s", request.InvestorID);
	printlc("td request.InstrumentID: %s", request.InstrumentID);
	request.HedgeFlag = THOST_FTDC_HF_Speculation;
	return api->ReqQryInstrumentMarginRate(&request, ++m_nRequestID);
}

int CTraderApi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) {
	return 0;
}
/********************************************************************************************************/
//call back 
/********************************************************************************************************/
void CTraderApi::OnFrontConnected() {
	printlb("td connected successfully.");
	printlb("td request user login from here.");
	ReqUserLogin();
}

void CTraderApi::OnFrontDisconnected(int nReason) {
	printlb("td connected failed, nReason: %d", nReason);
}

void CTraderApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("td OnRspUserLogin called.");
	printlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("td pRspInfo is NULL");
	}
	if (pRspUserLogin != NULL) {
		printlc("td pRspUserLogin->TradingDay: %s", pRspUserLogin->TradingDay);
		printlc("td pRspUserLogin->LoginTime: %s", pRspUserLogin->LoginTime);
		printlc("td pRspUserLogin->BrokerID: %s", pRspUserLogin->BrokerID);
		printlc("td pRspUserLogin->UserID: %s", pRspUserLogin->UserID); 
		printlc("td pRspUserLogin->SystemName: %s", pRspUserLogin->SystemName);
		printlc("td pRspUserLogin->FrontID: %d", pRspUserLogin->FrontID);
		printlc("td pRspUserLogin->SessionID: %d", pRspUserLogin->SessionID);
		printlc("td pRspUserLogin->MaxOrderRef: %s", pRspUserLogin->MaxOrderRef);
		printlc("td pRspUserLogin->SHFETime: %s", pRspUserLogin->SHFETime); 
		printlc("td pRspUserLogin->DCETime: %s", pRspUserLogin->DCETime);
		printlc("td pRspUserLogin->CZCETime: %s", pRspUserLogin->CZCETime);
		printlc("td pRspUserLogin->FFEXTime: %s", pRspUserLogin->FFEXTime);
		printlc("td pRspUserLogin->INETime: %s", pRspUserLogin->INETime);
	}
	else {
		printlc("td pRspUserLogin is NULL");
	}
}

void CTraderApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printlb("td OnRspOrderInsert called.");
	printlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("td pRspInfo is NULL");
	}
	if (pInputOrder != NULL) {
	}
	else {
		printlc("td pInputOrder is NULL");
	}
}

void CTraderApi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	printlb("td OnRtnOrder called");
}

void CTraderApi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	printlb("td OnRtnTrade called");
}

void CTraderApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	printlb("td OnRspQryInstrumentCommissionRate called.");
	printlc("td nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("td pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("td pRspInfo is NULL");
	}
	if (pInstrumentCommissionRate != NULL) {
		printlb("InstrumentID: %s", pInstrumentCommissionRate->InstrumentID);
		printlb("InvestorRange: %d", pInstrumentCommissionRate->InvestorRange);
		printlb("BrokerID: %s, InvestorID: %s", pInstrumentCommissionRate->BrokerID, pInstrumentCommissionRate->InvestorID);
		printlb("OpenRatioByMoney: %f", pInstrumentCommissionRate->OpenRatioByMoney);
		printlb("OpenRatioByVolume: %f", pInstrumentCommissionRate->OpenRatioByVolume);
		printlb("CloseRatioByMoney: %f", pInstrumentCommissionRate->CloseRatioByMoney);
		printlb("CloseRatioByVolume: %f", pInstrumentCommissionRate->CloseRatioByVolume);
		printlb("CloseTodayRatioByMoney: %f", pInstrumentCommissionRate->CloseTodayRatioByMoney);
		printlb("CloseTodayRatioByVolume: %f", pInstrumentCommissionRate->CloseTodayRatioByVolume);
	}
	else {
		printlc("td pInstrumentCommissionRate is NULL");
	}
}
