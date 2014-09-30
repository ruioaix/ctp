#include "ctpcapi.h"
#include <cstddef>

/********************************************************************************************************/
/***********MD*******************************************************************************************/
/********************************************************************************************************/
#include "mduserapi.h"
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

int MD_isready(void *md) {
	if (md) {
		return MD_GetApi(md)->IsReady();
	}
	return 0;
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

void MD_registerFensUserInfo(void *md) {
	if(md) {
		MD_GetApi(md)->RegisterFensUserInfo();
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

CThostFtdcDepthMarketDataField *MD_getOneDMDmsg(void *md, long *ts, long *tus, int *size) {
	if(md) {
		return MD_GetApi(md)->output_DMDQ(ts, tus, size);
	}
	return NULL;
}

CThostFtdcDepthMarketDataField *MD_2_getOneDMDmsg(void *md) {
	if(md) {
		return MD_GetApi(md)->output_slow_DMDQ();
	}
	return NULL;
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
void MD_RegOnRtnDepthMarketData(void* md, fnOnRtnDepthMarketData pCallback) {
	if(md) {
		MD_GetApi(md)->RegisterCallback_ordmd(pCallback);
	}
}

/********************************************************************************************************/
/******TD************************************************************************************************/
/********************************************************************************************************/
#include "traderapi.h"
static inline CTraderApi* TD_GetApi(void* td) {
	return static_cast<CTraderApi*>(td);
}

void* TD_create(char *flowpath, char *servername, char *bid, char *iid, char *uid, char *pd, char *UserProductInfo, THOST_TE_RESUME_TYPE nResumeType) {
	return new CTraderApi(flowpath, servername, bid, iid, uid, pd, UserProductInfo, nResumeType);
}

void TD_free(void* td) {
	if (td) {
		delete MD_GetApi(td);
	}
}

int TD_isready(void *td) {
	if (td) {
		return TD_GetApi(td)->IsReady();
	}
	return 0;
}

void TD_init(void *td) {
	if (td) {
		TD_GetApi(td)->Init();
	}
}

const char *TD_getTradingDay(void *td) {
	if (td) {
		return TD_GetApi(td)->GetTradingDay();
	}
	return NULL;
}

int TD_reqOrderInsert(void *td, int OrderRef, char *InstrumentID, \
		TThostFtdcOrderPriceTypeType OrderPriceType,\
		TThostFtdcOffsetFlagType CombOffsetFlag,\
		TThostFtdcDirectionType Direction,\
		TThostFtdcVolumeType VolumeTotalOriginal,\
		TThostFtdcPriceType LimitPrice,\
		TThostFtdcTimeConditionType TimeCondition,\
		TThostFtdcVolumeConditionType VolumeCondition) {

	if (td) {
		return TD_GetApi(td)->ReqOrderInsert(OrderRef, InstrumentID, OrderPriceType, CombOffsetFlag, Direction, VolumeTotalOriginal, LimitPrice, TimeCondition, VolumeCondition);
	}
	return -1;
}

int TD_limitOrder_open_buy(void *td, int OrderRef, char *InstrumentID, TThostFtdcVolumeType volume, TThostFtdcPriceType limitprice, TThostFtdcTimeConditionType timecond, TThostFtdcVolumeConditionType volumecond) {
	return TD_reqOrderInsert(td, OrderRef, InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_Open, THOST_FTDC_D_Buy, volume, limitprice, timecond, volumecond);
}
int TD_limitOrder_sell_closeToday(void *td, int OrderRef, char *InstrumentID, TThostFtdcVolumeType volume, TThostFtdcPriceType limitprice, TThostFtdcTimeConditionType timecond, TThostFtdcVolumeConditionType volumecond) {
	return TD_reqOrderInsert(td, OrderRef, InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_CloseToday, THOST_FTDC_D_Sell, volume, limitprice, timecond, volumecond);
}
int TD_limitOrder_sell_closeYesterday(void *td, int OrderRef, char *InstrumentID, TThostFtdcVolumeType volume, TThostFtdcPriceType limitprice, TThostFtdcTimeConditionType timecond, TThostFtdcVolumeConditionType volumecond) {
	return TD_reqOrderInsert(td, OrderRef, InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_Close, THOST_FTDC_D_Sell, volume, limitprice, timecond, volumecond);
}

int TD_limitOrder_open_sell(void *td, int OrderRef, char *InstrumentID, TThostFtdcVolumeType volume, TThostFtdcPriceType limitprice, TThostFtdcTimeConditionType timecond, TThostFtdcVolumeConditionType volumecond) {
	return TD_reqOrderInsert(td, OrderRef, InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_Open, THOST_FTDC_D_Sell, volume, limitprice, timecond, volumecond);
}
int TD_limitOrder_buy_closeToday(void *td, int OrderRef, char *InstrumentID, TThostFtdcVolumeType volume, TThostFtdcPriceType limitprice, TThostFtdcTimeConditionType timecond, TThostFtdcVolumeConditionType volumecond) {
	return TD_reqOrderInsert(td, OrderRef, InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_CloseToday, THOST_FTDC_D_Buy, volume, limitprice, timecond, volumecond);
}
int TD_limitOrder_buy_closeYesterday(void *td, int OrderRef, char *InstrumentID, TThostFtdcVolumeType volume, TThostFtdcPriceType limitprice, TThostFtdcTimeConditionType timecond, TThostFtdcVolumeConditionType volumecond) {
	return TD_reqOrderInsert(td, OrderRef, InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_Close, THOST_FTDC_D_Buy, volume, limitprice, timecond, volumecond);
}

int TD_reqQryInstrumentMarginRate(void *td, char *InstrumentID) {
	if (td) {
		return TD_GetApi(td)->ReqQryInstrumentMarginRate(InstrumentID);
	}
	return -1;
}

int TD_reqQrySettlementInfo(void *td) {
	if (td) {
		return TD_GetApi(td)->ReqQrySettlementInfo();
	}
	return -1;
}
int TD_reqQryInstrument(void *td) {
	if (td) {
		return TD_GetApi(td)->ReqQryInstrument();
	}
	return -1;
}
