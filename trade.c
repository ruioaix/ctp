// tradeapitest.cpp :
// 一个简单的例子,介绍CThostFtdcMdApi和CThostFtdcMdSpi接口的使用。
// 本例将演示一个报单录入操作的过程
#include <stdio.h>
//#include <windows.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "ThostFtdcMdApi.h"
// 报单录入操作是否完成的标志
// Create a manual reset event with no signal
//HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
// 会员代码
TThostFtdcBrokerIDType g_chBrokerID;
// 交易用户代码
TThostFtdcUserIDType g_chUserID;

class CSimpleHandler : public CThostFtdcMdSpi
{
	public:
		// 构造函数,需要一个有效的指向CThostFtdcMdApi实例的指针
		CSimpleHandler(CThostFtdcMdApi *pUserApi) : m_pUserApi(pUserApi) {}
		~CSimpleHandler() {}
		// 当客户端与交易托管系统建立起通信连接,客户端需要进行登录
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
			// 发出登陆请求
			m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
		}
		// 当客户端与交易托管系统通信连接断开时,该方法被调用
		virtual void OnFrontDisconnected(int nReason)
		{
			// 当发生这个情况后,API会自动重新连接,客户端可不做处理
			printf("OnFrontDisconnected.\n");
		}

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf("OnRspUserLogin:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
			if (pRspInfo->ErrorID != 0) {
				//端登失败,客户端需进行错误处理
				printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d", pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
				exit(-1);
			}
			// 端登成功
			// 订阅行情
			char * Instrumnet[]={"IF0809","IF0812"};
			m_pUserApi->SubscribeMarketData (Instrumnet,2);
			//或退订行情
			m_pUserApi->UnSubscribeMarketData (Instrumnet,2);
		}
		// 行情应答
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
		{
			// 输出报单录入结果
			printf("xx\n");fflush(stdout);
			//printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			//收到深度行情
			//SetEvent(g_hEvent);
		};
		// 针对用户请求的出错通知
		virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
			printf("OnRspError:\n");
			printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
			printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
			// 客户端需进行错误处理
			//{客户端的错误处理}

		}
	private:
		//指向CThostFtdcMdApi实例的指针
		CThostFtdcMdApi *m_pUserApi;
};

int main()
{
	// 产生一个CThostFtdcMdApi实例
	CThostFtdcMdApi *pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	// 产生一个事件处理的实例
	CSimpleHandler sh(pUserApi);
	// 注册一事件处理的实例
	pUserApi->RegisterSpi(&sh);
	// 设置交易托管系统服务的地址,可以注册多个地址备用
	pUserApi->RegisterFront("tcp://172.16.0.31:57205");
	// 使客户端开始与后台服务建立连接
	pUserApi->Init();
	// 客户端等待报单操作完成
	//WaitForSingleObject(g_hEvent, INFINITE);
	// 释放API实例
	
	sleep(10);
	pUserApi->Release();
	return 0;
}
