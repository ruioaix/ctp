#ifndef CTP_MD_SERVER_MDCALLBACK_H
#define CTP_MD_SERVER_MDCALLBACK_H

void OnFrontConnected_d(void* md);
void OnFrontDisconnected_d(void* md, int nReason);
void OnHeartBeatWarning_d(void* md, int nTimeLapse);
void OnRspUserLogin_d(void *md, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void OnRspUserLogout_d(void *md, CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void OnRspError_d(void* md, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void OnRspSubMarketData_d(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void OnRspUnSubMarketData_d(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void OnRtnDepthMarketData_d(void* md, CThostFtdcDepthMarketDataField *pDepthMarketData);

#endif
