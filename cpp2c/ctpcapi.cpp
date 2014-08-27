#include "ctpcapi.h"
#include "mduserapi.h"
#include <cstddef>

static inline CMdUserApi* MD_GetApi(void* md) {
	return static_cast<CMdUserApi*>(md);
}

void* MD_create(char *flowpath, char *servername, char *bid, char *iid, char *pd, char **InstrumentIDs, int InstrumentNum) {
	return new CMdUserApi(flowpath, servername, bid, iid, pd, InstrumentIDs, InstrumentNum);
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

void MD_init(void* md) {
	if(md) {
		MD_GetApi(md)->Init();
	}
}

void MD_join(void* md) {
	if(md) {
		MD_GetApi(md)->Join();
	}
}

void MD_getTradingDay(void* md) {
	if(md) {
		MD_GetApi(md)->GetTradingDay();
	}
}

void MD_registerFront(void *md, char *pszFrontAddress) {
	if(md) {
		MD_GetApi(md)->RegisterFront(pszFrontAddress);
	}
}

void MD_registerNameServer(void *md, char *pszNsAddress) {
	if(md) {
		MD_GetApi(md)->RegisterNameServer(pszNsAddress);
	}
}

void MD_registerFensUserInfo(void *md, CThostFtdcFensUserInfoField * pFensUserInfo) {
	if(md) {
		MD_GetApi(md)->RegisterFensUserInfo(pFensUserInfo);
	}
}

void MD_registerSpi(void *md) {
	if(md) {
		MD_GetApi(md)->RegisterSpi();
	}
}

int MD_subscribeMarketData(void *md, char *ppInstrumentID[], int nCount) {
	if(md) {
		return MD_GetApi(md)->SubscribeMarketData(ppInstrumentID, nCount);
	}
	return -1;
}

int MD_unSubscribeMarketData(void *md, char *ppInstrumentID[], int nCount) {
	if(md) {
		return MD_GetApi(md)->UnSubscribeMarketData(ppInstrumentID, nCount);
	}
	return -1;
}

int MD_subscribeForQuoteRsp(void *md, char *ppInstrumentID[], int nCount) {
	if(md) {
		return MD_GetApi(md)->SubscribeForQuoteRsp(ppInstrumentID, nCount);
	}
	return -1;
}

int MD_unSubscribeForQuoteRsp(void *md, char *ppInstrumentID[], int nCount) {
	if(md) {
		return MD_GetApi(md)->UnSubscribeForQuoteRsp(ppInstrumentID, nCount);
	}
	return -1;
}

int MD_reqUserLogin(void *md) {
	if(md) {
		return MD_GetApi(md)->ReqUserLogin();
	}
	return -1;
}

int MD_reqUserLogout(void *md) {
	if(md) {
		return MD_GetApi(md)->ReqUserLogout();
	}
	return -1;
}

CThostFtdcDepthMarketDataField *MD_getOneDMDmsg(void *md, double *arrivetime) {
	if(md) {
		return MD_GetApi(md)->output_DMDQ(arrivetime);
	}
	return NULL;
}
