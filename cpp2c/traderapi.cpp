#include "traderapi.h"
#include "base.h"
#include <string.h>
#include <sys/stat.h>

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

	mkdir(m_logFilePath, 0777);
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
	if (api) {
		api->RegisterSpi(NULL);
		api->Release();
		api=NULL;
	}
	printlb("delete md");
}

void CTraderApi::Init(void) {
	printlb("api init");
	api->Init();
}

void CTraderApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printlb("api sub public topic, TYPE: %d", nResumeType);
	api->SubscribePublicTopic(nResumeType);
}

void CTraderApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) {
	printlb("api sub private topic, TYPE: %d", nResumeType);
	api->SubscribePrivateTopic(nResumeType);
}

int CTraderApi::ReqUserLogin() {
	printlb("api req user login");

	CThostFtdcReqUserLoginField request;
	strncpy(request.BrokerID, m_BrokerId, sizeof(TThostFtdcBrokerIDType));
	strncpy(request.UserID, m_InvestorId, sizeof(TThostFtdcInvestorIDType));
	strncpy(request.Password, m_Password, sizeof(TThostFtdcPasswordType));
	printlc("request.BrokerID: %s", request.BrokerID);
	printlc("request.UserID: %s", request.UserID);
	printlc("request.Password: %s", request.Password);

	return api->ReqUserLogin(&request,++m_nRequestID);
}

int CTraderApi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField body;

	strncpy(body.BrokerID, m_BrokerId,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_InvestorId,sizeof(TThostFtdcInvestorIDType));
	return api->ReqSettlementInfoConfirm(&body, ++m_nRequestID);
}


void CTraderApi::OnFrontConnected() {
	printlb("connected successfully.");
	printlb("request user login from here.");
	ReqUserLogin();
}

void CTraderApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	printlb("OnRspUserLogin called.");
	printlc("nRequestId: %d, bIsLast: %d", nRequestID, bIsLast);
	if (pRspInfo != NULL) {
		printlc("pRspInfo->ErrorID: %x, pRspInfo->ErrorMsg: %s", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
	else {
		printlc("pRspInfo is NULL");
	}
	if (pRspUserLogin != NULL) {
		printlc("pRspUserLogin->TradingDay: %s", pRspUserLogin->TradingDay);
		printlc("pRspUserLogin->LoginTime: %s", pRspUserLogin->LoginTime);
		printlc("pRspUserLogin->BrokerID: %s", pRspUserLogin->BrokerID);
		printlc("pRspUserLogin->UserID: %s", pRspUserLogin->UserID); 
		printlc("pRspUserLogin->SystemName: %s", pRspUserLogin->SystemName);
		printlc("pRspUserLogin->FrontID: %d", pRspUserLogin->FrontID);
		printlc("pRspUserLogin->SessionID: %d", pRspUserLogin->SessionID);
		printlc("pRspUserLogin->MaxOrderRef: %s", pRspUserLogin->MaxOrderRef);
		printlc("pRspUserLogin->SHFETime: %s", pRspUserLogin->SHFETime); 
		printlc("pRspUserLogin->DCETime: %s", pRspUserLogin->DCETime);
		printlc("pRspUserLogin->CZCETime: %s", pRspUserLogin->CZCETime);
		printlc("pRspUserLogin->FFEXTime: %s", pRspUserLogin->FFEXTime);
		printlc("pRspUserLogin->INETime: %s", pRspUserLogin->INETime);
	}
	else {
		printlc("pRspUserLogin is NULL");
	}
	if ((pRspInfo==NULL || pRspInfo->ErrorID == 0) && pRspUserLogin) {
		printlb("req settlement info confirm from here");
		ReqSettlementInfoConfirm();
	}
}

