// tradeapitest.cpp :
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
//#include <windows.h>
#include "ThostFtdcMdApi.h"
// the flag whether the quotation data received or not.
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
// participant ID

#define bid 1035
#define uid "00000008"
#define pswd 123456

char g_chBrokerID[100];
// user id
char g_chUserID[100];
char g_pswd[100];

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
			char * Instrumnet[]={"IF1409","IF1410"};
			m_pUserApi->SubscribeMarketData(Instrumnet,2);
			//or unsubscribe the quotation
			//m_pUserApi->UnSubscribeMarketData (Instrumnet,2);
			m_pUserApi->SubscribeForQuoteRsp(Instrumnet, 2);
		}

		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf("OnRspSubMarketData:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
		}

		virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf("OnRtnForQuoteRsp:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
		}

		// quotation return
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
		{
			//output the order insert result
			printf("DMD InstrumentID: %s, updattime: %s, milltime: %d\n", pDepthMarketData->InstrumentID, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec);fflush(stdout);
			// set the flag when the quotation data received.
			//SetEvent(g_hEvent);
		}

		virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
		{
			printf("FQR InstrumentID: %s\n", pForQuoteRsp->InstrumentID);
		}
		
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
	// create a CThostFtdcMdApi instance
	CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	// create an event handler instance
	CSimpleHandler sh(pUserApi);
	// register an event handler instance
	pUserApi->RegisterSpi(&sh);

	// register the CTP front address and port
	pUserApi->RegisterFront("tcp://27.17.62.149:40213");
	//pUserApi->RegisterFront("tcp://222.66.97.241:41213");
	//pUserApi->RegisterFront("tcp://172.16.0.31:57205");

	// start the connection between client and CTP server
	pUserApi->Init();

	sleep(10);	

	// waiting for the quotation data
	//WaitForSingleObject(g_hEvent, INFINITE);
	// release API instance
	//pUserApi->Join();
	pUserApi->Release();

	return 0;
}
