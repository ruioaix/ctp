// tradeapitest.cpp :
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <windows.h>
#include "ThostFtdcMdApi.h"
// the flag whether the quotation data received or not.
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
// participant ID
TThostFtdcBrokerIDType g_chBrokerID;
// user id
TThostFtdcUserIDType g_chUserID;

class CSimpleHandler : public CThostFtdcMdSpi
{
	public:
		// constructor,which need a valid pointer of a CThostFtdcMdApi instance
		CSimpleHandler(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi) {}
		~CSimpleHandler() {}

		// when the connection between client and CTP server is created successfully, the client would send the login request to the CTP server.
		virtual void OnFrontConnected()
		{
			CThostFtdcReqUserLoginField reqUserLogin;
			// get BrokerID
			printf("BrokerID:");
			scanf("%s", &g_chBrokerID);
			strcpy(reqUserLogin. BrokerID, g_chBrokerID);
			// get userid
			printf("userid:");
			scanf("%s", &g_chUserID);
			strcpy(reqUserLogin.UserID, g_chUserID);
			// get password
			printf("password:");
			scanf("%s", &reqUserLogin.Password);
			// send the login request
			m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
		}

		// when client and CTP server disconnected,the follwing function will be called.
		virtual void OnFrontDisconnected(int nReason)
		{
			// inhtis case, API will reconnect,the client application canignore this.
			printf("OnFrontDisconnected.\n");
		}

		// after receiving the login request from the client,the CTP server will send the following response to notify the client whether the login success or not.
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf("OnRspUserLogin:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
			if (pRspInfo->ErrorID != 0) {
				// login failure, the client should handle this error.
				printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
				exit(-1);
			}
			// login success, then subscribe the quotation information
			char * Instrumnet[]={"IF0809","IF0812"};
			m_pUserApi->SubscribeMarketData (Instrumnet,2);
			//or unsubscribe the quotation
			m_pUserApi->UnSubscribeMarketData (Instrumnet,2);
		}

		// quotation return
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo)
		{
			//output the order insert result
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			// set the flag when the quotation data received.
			//SetEvent(g_hEvent);
		};

		// the error notification caused by client request
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
			printf("OnRspError:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

			// the client should handle the error
			//{error handle code}
		}
	private:
		// a pointer to CThostFtdcMdApi instance
		CThostFtdcMdApi *m_pUserApi;
};

int main()
{
	printf("begin\n");
	// create a CThostFtdcMdApi instance
	CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	// create an event handler instance
	CSimpleHandler sh(pUserApi);
	// register an event handler instance
	pUserApi->RegisterSpi(&sh);

	// register the CTP front address and port
	pUserApi->RegisterFront("tcp://222.66.97.241:41213");
	// start the connection between client and CTP server
	pUserApi->Init();

	// waiting for the quotation data
	//WaitForSingleObject(g_hEvent, INFINITE);
	// release API instance
	pUserApi->Release();
	return 0;
}
