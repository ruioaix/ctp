#include "header.h"
#include "MdUserApi.h"

inline CMdUserApi* MD_GetApi(void* pMdUserApi)
{
	return static_cast<CMdUserApi*>(pMdUserApi);
}

void MD_RegOnFrontConnected(void* pApi, fnOnFrontConnected pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_ofc(pCallback);
	}
}
void MD_RegOnFrontDisconnected(void* pApi, fnOnFrontDisconnected pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_ofd(pCallback);
	}
}
void MD_RegOnHeartBeatWarning(void* pApi, fnOnHeartBeatWarning pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_ohb(pCallback);
	}
}
void MD_RegOnRspUserLogin(void *pApi, fnOnRspUserLogin pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_oruli(pCallback);
	}
}
void MD_RegOnRspUserLogout(void *pApi, fnOnRspUserLogout pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_orulo(pCallback);
	}
}
void MD_RegOnRspError(void* pApi, fnOnRspError pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_ore(pCallback);
	}
}
void MD_RegOnRspSubMarketData(void *pApi, fnOnRspSubMarketData pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_orsmd(pCallback);
	}
}
void MD_RegOnRspUnSubMarketData(void *pApi, fnOnRspUnSubMarketData pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_orusmd(pCallback);
	}
}
void MD_RegOnRspSubForQuoteRsp(void *pApi, fnOnRspSubForQuoteRsp pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_orsfqr(pCallback);
	}
}
void MD_RegOnRspUnSubForQuoteRsp(void *pApi, fnOnRspUnSubForQuoteRsp pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_orusfqr(pCallback);
	}
}
void MD_RegOnRtnDepthMarketData(void* pApi, fnOnRtnDepthMarketData pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_ordmd(pCallback);
	}
}
void MD_RegOnRtnForQuoteRsp(void* pApi, fnOnRtnForQuoteRsp pCallback)
{
	if(pApi)
	{
		MD_GetApi(pApi)->RegisterCallback_orfqr(pCallback);
	}
}


void MD_Connect(void* pMdUserApi,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword)
{
	if(pMdUserApi
		&&szPath
		&&szAddresses
		&&szBrokerId
		&&szInvestorId
		&&szPassword)
	{
		printf("xxx\n");
		MD_GetApi(pMdUserApi)->Connect(szPath,szAddresses,szBrokerId,szInvestorId,szPassword);
	}
}

void* MD_CreateMdApi()
{
	return new CMdUserApi();
}

void MD_Subscribe(void* pMdUserApi,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pMdUserApi
		&&szInstrumentIDs)
	{
		MD_GetApi(pMdUserApi)->Subscribe(szInstrumentIDs);
	}
}

