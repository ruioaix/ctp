// tradeapitest.cpp :
// A simple example demonstrate how to use CThostFtdcTraderApi and CThostFtdcTraderSpi interface application.
// This example will demonstrates the procedure of an order insertion.
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
//#include <windows.h>
#include "ThostFtdcTraderApi.h"
// Flag of the order insertion finished or not.
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);

#define bid 9016766
#define uid 9016766
#define pswd 1111111

char g_chBrokerID[100];
// user id
char g_chUserID[100];
char g_pswd[100];


class CSimpleHandler : public CThostFtdcTraderSpi
{
	public:
		// constructor,which need a valid pointer to a CThostFtdcMduserApi instance
		CSimpleHandler(CThostFtdcTraderApi *pUserApi) : m_pUserApi(pUserApi) {}
		~CSimpleHandler() {}
		// After making a succeed connection with the CTP server, the client should send the login request to the CTP server.
		virtual void OnFrontConnected()
		{
			CThostFtdcReqUserLoginField reqUserLogin;
			// get BrokerID
			printf("BrokerID: %d\n", bid); fflush(stdout);
			sprintf(g_chBrokerID, "%d", bid);
			strcpy(reqUserLogin.BrokerID, g_chBrokerID);
			// get userid
			printf("userid: %d\n", uid); fflush(stdout);
			sprintf(g_chUserID, "%d", uid);
			strcpy(reqUserLogin.UserID, g_chUserID);
			// get password
			printf("password: %d\n", pswd); fflush(stdout);
			sprintf(g_pswd, "%d", pswd);
			strcpy(reqUserLogin.Password, g_pswd);
			// send the login request
			m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
		}
		//When the connection between client and the CTP server disconnected,the follwing function will be called.
		virtual void OnFrontDisconnected(int nReason)
		{
			// Inthis case, API willreconnect,the client application can ignore this.
			printf("OnFrontDisconnected.\n");
		}
		// After receiving the login request from the client,the CTP server will send the following response to notify the client whether the login success or not.
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf("OnRspUserLogin:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
			if (pRspInfo->ErrorID != 0) {
				// in case any login failure, the client should handle this error.
				printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
				exit(-1);
			}
			// login success, then send order insertion request.
			//CThostFtdcInputOrderField ord;
			//memset(&ord, 0, sizeof(ord));
			////broker id
			//strcpy(ord.BrokerID, g_chBrokerID);
			////investor ID
			//strcpy(ord.InvestorID, "12345");
			//// instrument ID
			//strcpy(ord.InstrumentID, "cn0601");
			/////order reference
			//strcpy(ord.OrderRef, "000000000001");
			//// user id
			//strcpy(ord.UserID, g_chUserID);
			//// order price type
			//ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			//// direction
			//ord.Direction = THOST_FTDC_D_Buy;
			//// combination order’s offset flag
			//strcpy(ord.CombOffsetFlag, "0");
			//// combination or hedge flag
			//strcpy(ord.CombHedgeFlag, "1");
			//// price
			//ord.LimitPrice = 50000;
			//// volume
			//ord.VolumeTotalOriginal = 10;
			//// valid date
			//ord.TimeCondition = THOST_FTDC_TC_GFD;
			//// GTD DATE
			//strcpy(ord.GTDDate, "");
			//// volume condition
			//ord.VolumeCondition = THOST_FTDC_VC_AV;
			//// min volume
			//ord.MinVolume = 0;
			//// trigger condition
			//ord.ContingentCondition = THOST_FTDC_CC_Immediately;
			//// stop price
			//ord.StopPrice = 0;
			//// force close reason
			//ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
			//// auto suspend flag
			//ord.IsAutoSuspend = 0;
			//m_pUserApi->ReqOrderInsert(&ord, 1);
		}
		// order insertion response
		virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			// output the order insertion result
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			// inform the main thread order insertion is over
			//SetEvent(g_hEvent);
		};
		///order insertion return
		virtual void OnRtnOrder(CThostFtdcOrderField *pOrder)
		{
			printf("OnRtnOrder:\n");
			printf("OrderSysID=[%s]\n", pOrder->OrderSysID);
		}
		// the error notification caused by client request
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
		{
			printf("OnRspError:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
			// the client should handle the error
			//{error handle code}
		}
		
		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
		{
			printf("InstrumentID: %s\n", pForQuoteRsp->InstrumentID);
		}
	private:
		// a pointer of CThostFtdcMduserApi instance
		CThostFtdcTraderApi *m_pUserApi;
};
int main()
{
	// create a CThostFtdcTraderApi instance
	CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	// create an event handler instance
	CSimpleHandler sh(pUserApi);
	// register an event handler instance
	pUserApi->RegisterSpi(&sh);
	// subscribe private topic
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	// subscribe public topic
	pUserApi->SubscribePublicTopic(THOST_TERT_RESUME);
	// register the CTP front address and port
	pUserApi->RegisterFront("tcp://222.66.97.241:41213");
	// make the connection between client and CTP server
	pUserApi->Init();
	// waiting for the order insertion.
	//WaitForSingleObject(g_hEvent, INFINITE);
	// release the API instance
	sleep(4);
	pUserApi->Release();
	return 0;
}
