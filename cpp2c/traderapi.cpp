#include "traderapi.h"

using namespace std;

CTraderApi::CTraderApi(char *flowpath, char *servername, char *brokerid, char *inverstorid, char *password, char ** InstrumentIDs, int InstrumentNum)
{
	/*
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

	m_InstrumentIDs = InstrumentIDs;
	m_InstrumentNum = InstrumentNum;
	int i;
	for (i = 0; i < m_InstrumentNum; ++i) {
		printlc("save Instrument %d: %s", i, m_InstrumentIDs[i]);
	}

	mkdir(m_logFilePath, 0777);
	api = CThostFtdcMdApi::CreateFtdcMdApi(m_logFilePath);
	if (api == NULL) {
		isError("mdapi created failed");
	}
	api->RegisterSpi(this);
	api->RegisterFront(m_server);

	//init 12 callback point.
	m_fnOnFrontConnected = NULL;
	m_fnOnFrontDisconnected = NULL;
	m_fnOnHeartBeatWarning = NULL;
	m_fnOnRspUserLogin = NULL;
	m_fnOnRspUserLogout = NULL;
	m_fnOnRspError = NULL;
	m_fnOnRspSubMarketData = NULL;
	m_fnOnRspUnSubMarketData = NULL;
	m_fnOnRspSubForQuoteRsp = NULL;
	m_fnOnRspUnSubForQuoteRsp = NULL;
	m_fnOnRtnDepthMarketData = NULL;
	m_fnOnRtnForQuoteRsp = NULL;

	m_queue_size = 8192;
	printlc("prepare msg queue, size: %d", m_queue_size);
	m_queue = (CThostFtdcDepthMarketDataField *)smalloc(m_queue_size * sizeof(CThostFtdcDepthMarketDataField));
	if (m_queue == NULL) {
		exit(-1);
	}
	m_intime_second = (long *)smalloc(m_queue_size * sizeof(long));
	if (m_intime_second == NULL) {
		exit(-1);
	}
	m_intime_usecond = (long *)smalloc(m_queue_size * sizeof(long));
	if (m_intime_usecond == NULL) {
		exit(-1);
	}
	m_current_size = (int *)smalloc(m_queue_size * sizeof(int));
	if (m_current_size == NULL) {
		exit(-1);
	}
	for (i = 0; i < m_queue_size; ++i) {
		m_intime_second[i] = -1;
		m_intime_usecond[i] = -1;
	}
	m_header_index = 0;
	m_tail_index = 0;
	m_validmsg_size = 0;
	*/
}
/*
#include "StdAfx.h"
#include "TraderApi.h"
#include "CTPMsgQueue.h"
#include "include\toolkit.h"

CTraderApi::CTraderApi(void)
{
	m_pApi = NULL;
	m_msgQueue = NULL;
	m_status = E_uninit;
	m_lRequestID = 0;

	m_hThread = NULL;
	m_bRunning = false;
}


CTraderApi::~CTraderApi(void)
{
	Disconnect();
}

void CTraderApi::StopThread()
{
	//停止发送线程
	m_bRunning = false;
	WaitForSingleObject(m_hThread,INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;
}

void CTraderApi::RegisterMsgQueue(CCTPMsgQueue* pMsgQueue)
{
	m_msgQueue = pMsgQueue;
}

bool CTraderApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if(bRet)
	{
		if(m_msgQueue)
			m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
	}
	return bRet;
}

bool CTraderApi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));

	return bRet;
}

void CTraderApi::Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword,
		THOST_TE_RESUME_TYPE nResumeType,
		const string& szUserProductInfo,
		const string& szAuthCode)
{
	m_szBrokerId = szBrokerId;
	m_szInvestorId = szInvestorId;
	m_szPassword = szPassword;
	m_szUserProductInfo = szUserProductInfo;
	m_szAuthCode = szAuthCode;

	char *pszPath = new char[szPath.length()+20];
	sprintf(pszPath,"%s\\Td\\",szPath.c_str());
	makedirs(pszPath);

	m_pApi = CThostFtdcTraderApi::CreateFtdcTraderApi(pszPath);
	delete[] pszPath;

	m_status = E_inited;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,m_status);
	
	if (m_pApi)
	{
		m_pApi->RegisterSpi(this);
		
		//添加地址
		size_t len = szAddresses.length()+1;
		char* buf = new char[len];
		strncpy(buf,szAddresses.c_str(),len);

		char* token = strtok(buf, _QUANTBOXC2CTP_SEPS_);
		while(token)
		{
			if (strlen(token)>0)
			{
				m_pApi->RegisterFront(token);
			}
			token = strtok( NULL, _QUANTBOXC2CTP_SEPS_);
		}
		delete[] buf;

		m_pApi->SubscribePublicTopic(nResumeType);
		m_pApi->SubscribePrivateTopic(nResumeType);
		
		//初始化连接
		m_pApi->Init();
		m_status = E_connecting;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);
	}
}

void CTraderApi::Disconnect()
{
	// 如果队列中有请求包，在后面又进行了Release,又回过头来发送，可能导致当了
	StopThread();

	m_status = E_unconnected;
	if(m_pApi)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;

		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,NULL,m_status);
	}

	m_lRequestID = 0;//由于线程已经停止，没有必要用原子操作了

	ReleaseRequestListBuf();
	ReleaseRequestMapBuf();
}

CTraderApi::SRequest* CTraderApi::MakeRequestBuf(RequestType type)
{
	SRequest *pRequest = new SRequest;
	if (NULL == pRequest)
		return NULL;

	memset(pRequest,0,sizeof(SRequest));
	pRequest->type = type;

	return pRequest;
}

void CTraderApi::ReleaseRequestListBuf()
{
	lock_guard<mutex> cl(m_csList);
	while (!m_reqList.empty())
	{
		SRequest * pRequest = m_reqList.front();
		delete pRequest;
		m_reqList.pop_front();
	}
}

void CTraderApi::ReleaseRequestMapBuf()
{
	lock_guard<mutex> cl(m_csMap);
	for (map<int,SRequest*>::iterator it=m_reqMap.begin();it!=m_reqMap.end();++it)
	{
		delete (*it).second;
	}
	m_reqMap.clear();
}

void CTraderApi::ReleaseRequestMapBuf(int nRequestID)
{
	lock_guard<mutex> cl(m_csMap);
	map<int,SRequest*>::iterator it = m_reqMap.find(nRequestID);
	if (it!=m_reqMap.end())
	{
		delete it->second;
		m_reqMap.erase(nRequestID);
	}
}

void CTraderApi::AddRequestMapBuf(int nRequestID,SRequest* pRequest)
{
	if(NULL == pRequest)
		return;

	lock_guard<mutex> cl(m_csMap);
	map<int,SRequest*>::iterator it = m_reqMap.find(nRequestID);
	if (it!=m_reqMap.end())
	{
		SRequest* p = it->second;
		if(pRequest != p)//如果实际上指的是同一内存，不再插入
		{
			delete p;
			m_reqMap[nRequestID] = pRequest;
		}
	}
}

DWORD WINAPI SendThread(LPVOID lpParam)
{
	CTraderApi* pTrade = reinterpret_cast<CTraderApi *>(lpParam);
	if (pTrade)
		pTrade->RunInThread();
	return 0;
}

void CTraderApi::AddToSendQueue(SRequest * pRequest)
{
	if (NULL == pRequest)
		return;

	lock_guard<mutex> cl(m_csList);
	bool bFind = false;
	//目前不去除相同类型的请求，即没有对大量同类型请求进行优化
	//for (list<SRequest*>::iterator it = m_reqList.begin();it!= m_reqList.end();++it)
	//{
	//	if (pRequest->type == (*it)->type)
	//	{
	//		bFind = true;
	//		break;
	//	}
	//}

	if (!bFind)
		m_reqList.push_back(pRequest);

	if (NULL == m_hThread
		&&!m_reqList.empty())
	{
		m_bRunning = true;
		m_hThread = CreateThread(NULL,0,SendThread,this,0,NULL); 
	}
}


void CTraderApi::RunInThread()
{
	int iRet = 0;

	while (!m_reqList.empty()&&m_bRunning)
	{
		SRequest * pRequest = m_reqList.front();
		int lRequest = ++m_lRequestID;// 这个地方是否会出现原子操作的问题呢？
		switch(pRequest->type)
		{
		case E_ReqAuthenticateField:
			iRet = m_pApi->ReqAuthenticate(&pRequest->ReqAuthenticateField,lRequest);
			break;
		case E_ReqUserLoginField:
			iRet = m_pApi->ReqUserLogin(&pRequest->ReqUserLoginField,lRequest);
			break;
		case E_SettlementInfoConfirmField:
			iRet = m_pApi->ReqSettlementInfoConfirm(&pRequest->SettlementInfoConfirmField,lRequest);
			break;
		case E_QryInstrumentField:
			iRet = m_pApi->ReqQryInstrument(&pRequest->QryInstrumentField,lRequest);
			break;
		case E_QryTradingAccountField:
			iRet = m_pApi->ReqQryTradingAccount(&pRequest->QryTradingAccountField,lRequest);
			break;
		case E_QryInvestorPositionField:
			iRet = m_pApi->ReqQryInvestorPosition(&pRequest->QryInvestorPositionField,lRequest);
			break;
		case E_QryInvestorPositionDetailField:
			iRet=m_pApi->ReqQryInvestorPositionDetail(&pRequest->QryInvestorPositionDetailField,lRequest);
			break;
		case E_QryInstrumentCommissionRateField:
			iRet = m_pApi->ReqQryInstrumentCommissionRate(&pRequest->QryInstrumentCommissionRateField,lRequest);
			break;
		case E_QryInstrumentMarginRateField:
			iRet = m_pApi->ReqQryInstrumentMarginRate(&pRequest->QryInstrumentMarginRateField,lRequest);
			break;
		case E_QryDepthMarketDataField:
			iRet = m_pApi->ReqQryDepthMarketData(&pRequest->QryDepthMarketDataField,lRequest);
			break;
		case E_QrySettlementInfoField:
			iRet = m_pApi->ReqQrySettlementInfo(&pRequest->QrySettlementInfoField, lRequest);
			break;
		default:
			_ASSERT(FALSE);
			break;
		}

		if (0 == iRet)
		{
			//返回成功，填加到已发送池
			m_nSleep = 1;
			AddRequestMapBuf(lRequest,pRequest);

			lock_guard<mutex> cl(m_csList);
			m_reqList.pop_front();
		}
		else
		{
			//失败，按4的幂进行延时，但不超过1s
			m_nSleep *= 4;
			m_nSleep %= 1023;
		}
		Sleep(m_nSleep);
	}

	//清理线程
	CloseHandle(m_hThread);
	m_hThread = NULL;
	m_bRunning = false;
}

void CTraderApi::OnFrontConnected()
{
	m_status =  E_connected;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,m_status);

	//连接成功后自动请求认证或登录
	if(m_szAuthCode.length()>0
		&&m_szUserProductInfo.length()>0)
	{
		//填了认证码就先认证
		ReqAuthenticate();
	}
	else
	{
		ReqUserLogin();
	}
}

void CTraderApi::OnFrontDisconnected(int nReason)
{
	m_status = E_unconnected;
	CThostFtdcRspInfoField RspInfo;
	//构造出来的错误消息，为了统一出错信息
	RspInfo.ErrorID = nReason;
	GetOnFrontDisconnectedMsg(&RspInfo);

	if(m_msgQueue)
		m_msgQueue->Input_OnDisconnect(this,&RspInfo,m_status);
}

void CTraderApi::ReqAuthenticate()
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_ReqAuthenticateField);
	if (pRequest)
	{
		m_status = E_authing;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		CThostFtdcReqAuthenticateField& body = pRequest->ReqAuthenticateField;

		strncpy(body.BrokerID, m_szBrokerId.c_str(),sizeof(TThostFtdcBrokerIDType));
		strncpy(body.UserID, m_szInvestorId.c_str(),sizeof(TThostFtdcInvestorIDType));
		strncpy(body.UserProductInfo,m_szUserProductInfo.c_str(),sizeof(TThostFtdcProductInfoType));
		strncpy(body.AuthCode,m_szAuthCode.c_str(),sizeof(TThostFtdcAuthCodeType));

		AddToSendQueue(pRequest);
	}
}

void CTraderApi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pRspAuthenticateField)
	{
		m_status = E_authed;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		ReqUserLogin();
	}
	else
	{
		m_status = E_connected;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_authing);
	}

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqUserLogin()
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_ReqUserLoginField);
	if (pRequest)
	{
		m_status = E_logining;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		CThostFtdcReqUserLoginField& body = pRequest->ReqUserLoginField;

		strncpy(body.BrokerID, m_szBrokerId.c_str(),sizeof(TThostFtdcBrokerIDType));
		strncpy(body.UserID, m_szInvestorId.c_str(),sizeof(TThostFtdcInvestorIDType));
		strncpy(body.Password, m_szPassword.c_str(),sizeof(TThostFtdcPasswordType));
		strncpy(body.UserProductInfo,m_szUserProductInfo.c_str(),sizeof(TThostFtdcProductInfoType));

		AddToSendQueue(pRequest);
	}
}

void CTraderApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pRspUserLogin)
	{
		m_status = E_logined;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,pRspUserLogin,m_status);
		
		memcpy(&m_RspUserLogin,pRspUserLogin,sizeof(CThostFtdcRspUserLoginField));
		m_nMaxOrderRef = atol(pRspUserLogin->MaxOrderRef);
		ReqSettlementInfoConfirm();
	}
	else
	{
		m_status = E_authed;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
	}

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqSettlementInfoConfirm()
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_SettlementInfoConfirmField);
	if (pRequest)
	{
		m_status = E_confirming;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		CThostFtdcSettlementInfoConfirmField& body = pRequest->SettlementInfoConfirmField;

		strncpy(body.BrokerID, m_szBrokerId.c_str(),sizeof(TThostFtdcBrokerIDType));
		strncpy(body.InvestorID, m_szInvestorId.c_str(),sizeof(TThostFtdcInvestorIDType));

		AddToSendQueue(pRequest);
	}
}

void CTraderApi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pSettlementInfoConfirm)
	{
		m_status = E_confirmed;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);
	}
	else
	{
		m_status = E_logined;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_confirming);
	}

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

int CTraderApi::ReqOrderInsert(
	int OrderRef,
	const string& szInstrumentId,
	TThostFtdcDirectionType Direction,
	const TThostFtdcCombOffsetFlagType CombOffsetFlag,
	const TThostFtdcCombHedgeFlagType CombHedgeFlag,
	TThostFtdcVolumeType VolumeTotalOriginal,
	TThostFtdcPriceType LimitPrice,
	TThostFtdcOrderPriceTypeType OrderPriceType,
	TThostFtdcTimeConditionType TimeCondition,
	TThostFtdcContingentConditionType ContingentCondition,
	TThostFtdcPriceType StopPrice,
	TThostFtdcVolumeConditionType VolumeCondition)
{
	if (NULL == m_pApi)
		return 0;

	SRequest* pRequest = MakeRequestBuf(E_InputOrderField);
	if (NULL == pRequest)
		return 0;

	CThostFtdcInputOrderField& body = pRequest->InputOrderField;
	
	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TThostFtdcInvestorIDType));
	
	body.MinVolume = 1;
	body.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	body.IsAutoSuspend = 0;
	body.UserForceClose = 0;
	body.IsSwapOrder = 0;

	//合约
	strncpy(body.InstrumentID, szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));
	//买卖
	body.Direction = Direction;
	//开平
	memcpy(body.CombOffsetFlag,CombOffsetFlag,sizeof(TThostFtdcCombOffsetFlagType));
	
	//价格
	body.OrderPriceType = OrderPriceType;
	body.LimitPrice = LimitPrice;		
	
	//数量
	body.VolumeTotalOriginal = VolumeTotalOriginal;
	//投保
	memcpy(body.CombHedgeFlag,CombHedgeFlag,sizeof(TThostFtdcCombHedgeFlagType));
	
	//各条件
	body.VolumeCondition = VolumeCondition;
	body.TimeCondition = TimeCondition;
	body.ContingentCondition = ContingentCondition;
	body.StopPrice = StopPrice;

	int nRet = 0;
	{
		//可能报单太快，m_nMaxOrderRef还没有改变就提交了
		lock_guard<mutex> cl(m_csOrderRef);

		if (OrderRef < 0)
		{
			nRet = m_nMaxOrderRef;
			sprintf(body.OrderRef, "%d", nRet);
			++m_nMaxOrderRef;
		}
		else
		{
			nRet = OrderRef;
			sprintf(body.OrderRef, "%d", OrderRef);
		}

		//不保存到队列，而是直接发送
		int n = m_pApi->ReqOrderInsert(&pRequest->InputOrderField, ++m_lRequestID);
		if (n < 0)
		{
			nRet = n;
		}
	}
	delete pRequest;//用完后直接删除

	//如何定位报单，用报单引用实际上并不靠谱
	//如重新开软件、条件单，同时别的软件下单三个可能性要处理
	return nRet;
}

void CTraderApi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspOrderInsert(this,pInputOrder,pRspInfo,nRequestID,bIsLast);
}

void CTraderApi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnErrRtnOrderInsert(this,pInputOrder,pRspInfo);
}

void CTraderApi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRtnTrade(this,pTrade);
}

int CTraderApi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	if (NULL == m_pApi)
		return 0;
	
	SRequest* pRequest = MakeRequestBuf(E_InputOrderActionField);
	if (NULL == pRequest)
		return 0;
	
	CThostFtdcInputOrderActionField& body = pRequest->InputOrderActionField;

	///经纪公司代码
	strncpy(body.BrokerID, pOrder->BrokerID,sizeof(TThostFtdcBrokerIDType));
	///投资者代码
	strncpy(body.InvestorID, pOrder->InvestorID,sizeof(TThostFtdcInvestorIDType));
	///报单引用
	strncpy(body.OrderRef, pOrder->OrderRef,sizeof(TThostFtdcOrderRefType));
	///前置编号
	body.FrontID = pOrder->FrontID;
	///会话编号
	body.SessionID = pOrder->SessionID;
	///交易所代码
	strncpy(body.ExchangeID,pOrder->ExchangeID,sizeof(TThostFtdcExchangeIDType));
	///报单编号
	strncpy(body.OrderSysID,pOrder->OrderSysID,sizeof(TThostFtdcOrderSysIDType));
	///操作标志
	body.ActionFlag = THOST_FTDC_AF_Delete;
	///合约代码
	strncpy(body.InstrumentID, pOrder->InstrumentID,sizeof(TThostFtdcInstrumentIDType));
	
	int nRet = m_pApi->ReqOrderAction(&pRequest->InputOrderActionField, ++m_lRequestID);
	delete pRequest;
	return nRet;
}

void CTraderApi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspOrderAction(this,pInputOrderAction,pRspInfo,nRequestID,bIsLast);
}

void CTraderApi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnErrRtnOrderAction(this,pOrderAction,pRspInfo);
}

void CTraderApi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRtnOrder(this,pOrder);
}

int CTraderApi::ReqQuoteInsert(
	int QuoteRef,
	const string& szInstrumentId,
	TThostFtdcPriceType	AskPrice,
	TThostFtdcPriceType	BidPrice,
	TThostFtdcVolumeType AskVolume,
	TThostFtdcVolumeType BidVolume,
	TThostFtdcOffsetFlagType AskOffsetFlag,
	TThostFtdcOffsetFlagType BidOffsetFlag,
	TThostFtdcHedgeFlagType	AskHedgeFlag,
	TThostFtdcHedgeFlagType	BidHedgeFlag
	)
{
	if (NULL == m_pApi)
		return 0;

	SRequest* pRequest = MakeRequestBuf(E_InputQuoteField);
	if (NULL == pRequest)
		return 0;

	CThostFtdcInputQuoteField& body = pRequest->InputQuoteField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID, sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID, sizeof(TThostFtdcInvestorIDType));

	//合约
	strncpy(body.InstrumentID, szInstrumentId.c_str(), sizeof(TThostFtdcInstrumentIDType));
	//开平
	body.AskOffsetFlag = AskOffsetFlag;
	body.BidOffsetFlag = BidOffsetFlag;
	//投保
	body.AskHedgeFlag = AskHedgeFlag;
	body.BidHedgeFlag = BidHedgeFlag;

	//价格
	body.AskPrice = AskPrice;
	body.BidPrice = BidPrice;

	//数量
	body.AskVolume = AskVolume;
	body.BidVolume = BidVolume;
	
	int nRet = 0;
	{
		//可能报单太快，m_nMaxOrderRef还没有改变就提交了
		lock_guard<mutex> cl(m_csOrderRef);

		if (QuoteRef < 0)
		{
			nRet = m_nMaxOrderRef;
			sprintf(body.QuoteRef, "%d", nRet);
			++m_nMaxOrderRef;
		}
		else
		{
			nRet = QuoteRef;
			sprintf(body.QuoteRef, "%d", QuoteRef);
		}

		//不保存到队列，而是直接发送
		int n = m_pApi->ReqQuoteInsert(&pRequest->InputQuoteField, ++m_lRequestID);
		if (n < 0)
		{
			nRet = n;
		}
	}
	delete pRequest;//用完后直接删除

	return nRet;
}

void CTraderApi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_msgQueue)
		m_msgQueue->Input_OnRspQuoteInsert(this, pInputQuote, pRspInfo, nRequestID, bIsLast);
}

void CTraderApi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo)
{
	if (m_msgQueue)
		m_msgQueue->Input_OnErrRtnQuoteInsert(this, pInputQuote, pRspInfo);
}

void CTraderApi::OnRtnQuote(CThostFtdcQuoteField *pQuote)
{
	if (m_msgQueue)
		m_msgQueue->Input_OnRtnQuote(this, pQuote);
}

int CTraderApi::ReqQuoteAction(CThostFtdcQuoteField *pQuote)
{
	if (NULL == m_pApi)
		return 0;

	SRequest* pRequest = MakeRequestBuf(E_InputQuoteActionField);
	if (NULL == pRequest)
		return 0;

	CThostFtdcInputQuoteActionField& body = pRequest->InputQuoteActionField;

	///经纪公司代码
	strncpy(body.BrokerID, pQuote->BrokerID, sizeof(TThostFtdcBrokerIDType));
	///投资者代码
	strncpy(body.InvestorID, pQuote->InvestorID, sizeof(TThostFtdcInvestorIDType));
	///报单引用
	strncpy(body.QuoteRef, pQuote->QuoteRef, sizeof(TThostFtdcOrderRefType));
	///前置编号
	body.FrontID = pQuote->FrontID;
	///会话编号
	body.SessionID = pQuote->SessionID;
	///交易所代码
	strncpy(body.ExchangeID, pQuote->ExchangeID, sizeof(TThostFtdcExchangeIDType));
	///报单编号
	strncpy(body.QuoteSysID, pQuote->QuoteSysID, sizeof(TThostFtdcOrderSysIDType));
	///操作标志
	body.ActionFlag = THOST_FTDC_AF_Delete;
	///合约代码
	strncpy(body.InstrumentID, pQuote->InstrumentID, sizeof(TThostFtdcInstrumentIDType));

	int nRet = m_pApi->ReqQuoteAction(&pRequest->InputQuoteActionField, ++m_lRequestID);
	delete pRequest;
	return nRet;
}

void CTraderApi::OnRspQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_msgQueue)
		m_msgQueue->Input_OnRspQuoteAction(this, pInputQuoteAction, pRspInfo, nRequestID, bIsLast);
}

void CTraderApi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo)
{
	if (m_msgQueue)
		m_msgQueue->Input_OnErrRtnQuoteAction(this, pQuoteAction, pRspInfo);
}

void CTraderApi::ReqQryTradingAccount()
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_QryTradingAccountField);
	if (NULL == pRequest)
		return;
	
	CThostFtdcQryTradingAccountField& body = pRequest->QryTradingAccountField;
	
	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TThostFtdcInvestorIDType));
	
	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryTradingAccount(this,pTradingAccount,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInvestorPosition(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_QryInvestorPositionField);
	if (NULL == pRequest)
		return;
	
	CThostFtdcQryInvestorPositionField& body = pRequest->QryInvestorPositionField;
	
	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));
	
	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInvestorPosition(this,pInvestorPosition,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInvestorPositionDetail(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryInvestorPositionDetailField);
	if (NULL == pRequest)
		return;

	CThostFtdcQryInvestorPositionDetailField& body = pRequest->QryInvestorPositionDetailField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInvestorPositionDetail(this,pInvestorPositionDetail,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInstrument(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_QryInstrumentField);
	if (NULL == pRequest)
		return;
	
	CThostFtdcQryInstrumentField& body = pRequest->QryInstrumentField;
	
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));
	
	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInstrument(this,pInstrument,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInstrumentCommissionRate(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryInstrumentCommissionRateField);
	if (NULL == pRequest)
		return;

	CThostFtdcQryInstrumentCommissionRateField& body = pRequest->QryInstrumentCommissionRateField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInstrumentCommissionRate(this,pInstrumentCommissionRate,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInstrumentMarginRate(const string& szInstrumentId,TThostFtdcHedgeFlagType HedgeFlag)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryInstrumentMarginRateField);
	if (NULL == pRequest)
		return;

	CThostFtdcQryInstrumentMarginRateField& body = pRequest->QryInstrumentMarginRateField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));
	body.HedgeFlag = HedgeFlag;

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInstrumentMarginRate(this,pInstrumentMarginRate,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryDepthMarketData(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryDepthMarketDataField);
	if (NULL == pRequest)
		return;

	CThostFtdcQryDepthMarketDataField& body = pRequest->QryDepthMarketDataField;

	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TThostFtdcInstrumentIDType));

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryDepthMarketData(this,pDepthMarketData,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_msgQueue)
		m_msgQueue->Input_OnRspQrySettlementInfo(this, pSettlementInfo, pRspInfo, nRequestID, bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryOrder(this,pOrder,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}


void CTraderApi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryTrade(this,pTrade,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRtnInstrumentStatus(this,pInstrumentStatus);
}

void CTraderApi::ReqQrySettlementInfo(const string& szTradingDay)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QrySettlementInfoField);
	if (NULL == pRequest)
		return;

	CThostFtdcQrySettlementInfoField& body = pRequest->QrySettlementInfoField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID, sizeof(TThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID, sizeof(TThostFtdcInvestorIDType));
	strncpy(body.TradingDay, szTradingDay.c_str(), sizeof(TThostFtdcDateType));

	AddToSendQueue(pRequest);
}
*/
