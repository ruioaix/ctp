#include "ctpcapi.h"
#include "mduserapi.h"

static inline CMdUserApi* MD_GetApi(void* md) {
	return static_cast<CMdUserApi*>(md);
}

void* MD_create(char *flowpath, char *servername, char *bid, char *iid, char *pd) {
	return new CMdUserApi(flowpath, servername, bid, iid, pd);
}

void MD_free(void* md) {
	if (md) {
		delete MD_GetApi(md);
	}
}

void MD_RegOnFrontConnected(void* md, fnOnFrontConnected pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_ofc(pCallback);
	}
}
void MD_RegOnFrontDisconnected(void* md, fnOnFrontDisconnected pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_ofd(pCallback);
	}
}
void MD_RegOnHeartBeatWarning(void* md, fnOnHeartBeatWarning pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_ohb(pCallback);
	}
}
void MD_RegOnRspUserLogin(void *md, fnOnRspUserLogin pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_oruli(pCallback);
	}
}
void MD_RegOnRspUserLogout(void *md, fnOnRspUserLogout pCallback) { if(md)
	{
		MD_GetApi(md)->RegisterCallback_orulo(pCallback);
	}
}
void MD_RegOnRspError(void* md, fnOnRspError pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_ore(pCallback);
	}
}
void MD_RegOnRspSubMarketData(void *md, fnOnRspSubMarketData pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_orsmd(pCallback);
	}
}
void MD_RegOnRspUnSubMarketData(void *md, fnOnRspUnSubMarketData pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_orusmd(pCallback);
	}
}
void MD_RegOnRspSubForQuoteRsp(void *md, fnOnRspSubForQuoteRsp pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_orsfqr(pCallback);
	}
}
void MD_RegOnRspUnSubForQuoteRsp(void *md, fnOnRspUnSubForQuoteRsp pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_orusfqr(pCallback);
	}
}
void MD_RegOnRtnDepthMarketData(void* md, fnOnRtnDepthMarketData pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_ordmd(pCallback);
	}
}
void MD_RegOnRtnForQuoteRsp(void* md, fnOnRtnForQuoteRsp pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_orfqr(pCallback);
	}
}

void MD_connect(void* md, const char* szPath, const char* szAddresses, const char* szBrokerId, const char* szInvestorId, const char* szPassword) {
	if(md && szPath && szAddresses && szBrokerId && szInvestorId && szPassword) {
		printf("xxx\n");
		MD_GetApi(md)->Connect();
	}
}

void MD_Disconnect(void* md) {
	if(md) {
		MD_GetApi(md)->Disconnect();
	}
}

void MD_Subscribe(void* md,const char* szInstrumentIDs,const char* szExchageID) {
	if(md && szInstrumentIDs) {
		MD_GetApi(md)->Subscribe(szInstrumentIDs);
	}
}

void  MD_Unsubscribe(void* md, const char* szInstrumentIDs, const char* szExchageID) {
	if(md && szInstrumentIDs)
	{
		MD_GetApi(md)->Unsubscribe(szInstrumentIDs);
	}
}

