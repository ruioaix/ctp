#ifndef CTP_C_API_HEADER_H
#define CTP_C_API_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

//In the following included file,  there are 281 basic structures.
#include "cpp_api/ThostFtdcUserApiStruct.h"
//281 typedef
typedef struct CThostFtdcDisseminationField			CThostFtdcDisseminationField;
typedef struct CThostFtdcReqUserLoginField			CThostFtdcReqUserLoginField;
typedef struct CThostFtdcRspUserLoginField			CThostFtdcRspUserLoginField;
typedef struct CThostFtdcUserLogoutField			CThostFtdcUserLogoutField;
typedef struct CThostFtdcForceUserLogoutField			CThostFtdcForceUserLogoutField;
typedef struct CThostFtdcReqAuthenticateField			CThostFtdcReqAuthenticateField;
typedef struct CThostFtdcRspAuthenticateField			CThostFtdcRspAuthenticateField;
typedef struct CThostFtdcAuthenticationInfoField			CThostFtdcAuthenticationInfoField;
typedef struct CThostFtdcTransferHeaderField			CThostFtdcTransferHeaderField;
typedef struct CThostFtdcTransferBankToFutureReqField			CThostFtdcTransferBankToFutureReqField;
typedef struct CThostFtdcTransferBankToFutureRspField			CThostFtdcTransferBankToFutureRspField;
typedef struct CThostFtdcTransferFutureToBankReqField			CThostFtdcTransferFutureToBankReqField;
typedef struct CThostFtdcTransferFutureToBankRspField			CThostFtdcTransferFutureToBankRspField;
typedef struct CThostFtdcTransferQryBankReqField			CThostFtdcTransferQryBankReqField;
typedef struct CThostFtdcTransferQryBankRspField			CThostFtdcTransferQryBankRspField;
typedef struct CThostFtdcTransferQryDetailReqField			CThostFtdcTransferQryDetailReqField;
typedef struct CThostFtdcTransferQryDetailRspField			CThostFtdcTransferQryDetailRspField;
typedef struct CThostFtdcRspInfoField			CThostFtdcRspInfoField;
typedef struct CThostFtdcExchangeField			CThostFtdcExchangeField;
typedef struct CThostFtdcProductField			CThostFtdcProductField;
typedef struct CThostFtdcInstrumentField			CThostFtdcInstrumentField;
typedef struct CThostFtdcBrokerField			CThostFtdcBrokerField;
typedef struct CThostFtdcTraderField			CThostFtdcTraderField;
typedef struct CThostFtdcInvestorField			CThostFtdcInvestorField;
typedef struct CThostFtdcTradingCodeField			CThostFtdcTradingCodeField;
typedef struct CThostFtdcPartBrokerField			CThostFtdcPartBrokerField;
typedef struct CThostFtdcSuperUserField			CThostFtdcSuperUserField;
typedef struct CThostFtdcSuperUserFunctionField			CThostFtdcSuperUserFunctionField;
typedef struct CThostFtdcInvestorGroupField			CThostFtdcInvestorGroupField;
typedef struct CThostFtdcTradingAccountField			CThostFtdcTradingAccountField;
typedef struct CThostFtdcInvestorPositionField			CThostFtdcInvestorPositionField;
typedef struct CThostFtdcInstrumentMarginRateField			CThostFtdcInstrumentMarginRateField;
typedef struct CThostFtdcInstrumentCommissionRateField			CThostFtdcInstrumentCommissionRateField;
typedef struct CThostFtdcDepthMarketDataField			CThostFtdcDepthMarketDataField;
typedef struct CThostFtdcInstrumentTradingRightField			CThostFtdcInstrumentTradingRightField;
typedef struct CThostFtdcBrokerUserField			CThostFtdcBrokerUserField;
typedef struct CThostFtdcBrokerUserPasswordField			CThostFtdcBrokerUserPasswordField;
typedef struct CThostFtdcBrokerUserFunctionField			CThostFtdcBrokerUserFunctionField;
typedef struct CThostFtdcTraderOfferField			CThostFtdcTraderOfferField;
typedef struct CThostFtdcSettlementInfoField			CThostFtdcSettlementInfoField;
typedef struct CThostFtdcInstrumentMarginRateAdjustField			CThostFtdcInstrumentMarginRateAdjustField;
typedef struct CThostFtdcExchangeMarginRateField			CThostFtdcExchangeMarginRateField;
typedef struct CThostFtdcExchangeMarginRateAdjustField			CThostFtdcExchangeMarginRateAdjustField;
typedef struct CThostFtdcExchangeRateField			CThostFtdcExchangeRateField;
typedef struct CThostFtdcSettlementRefField			CThostFtdcSettlementRefField;
typedef struct CThostFtdcCurrentTimeField			CThostFtdcCurrentTimeField;
typedef struct CThostFtdcCommPhaseField			CThostFtdcCommPhaseField;
typedef struct CThostFtdcLoginInfoField			CThostFtdcLoginInfoField;
typedef struct CThostFtdcLogoutAllField			CThostFtdcLogoutAllField;
typedef struct CThostFtdcFrontStatusField			CThostFtdcFrontStatusField;
typedef struct CThostFtdcUserPasswordUpdateField			CThostFtdcUserPasswordUpdateField;
typedef struct CThostFtdcInputOrderField			CThostFtdcInputOrderField;
typedef struct CThostFtdcOrderField			CThostFtdcOrderField;
typedef struct CThostFtdcExchangeOrderField			CThostFtdcExchangeOrderField;
typedef struct CThostFtdcExchangeOrderInsertErrorField			CThostFtdcExchangeOrderInsertErrorField;
typedef struct CThostFtdcInputOrderActionField			CThostFtdcInputOrderActionField;
typedef struct CThostFtdcOrderActionField			CThostFtdcOrderActionField;
typedef struct CThostFtdcExchangeOrderActionField			CThostFtdcExchangeOrderActionField;
typedef struct CThostFtdcExchangeOrderActionErrorField			CThostFtdcExchangeOrderActionErrorField;
typedef struct CThostFtdcExchangeTradeField			CThostFtdcExchangeTradeField;
typedef struct CThostFtdcTradeField			CThostFtdcTradeField;
typedef struct CThostFtdcUserSessionField			CThostFtdcUserSessionField;
typedef struct CThostFtdcQueryMaxOrderVolumeField			CThostFtdcQueryMaxOrderVolumeField;
typedef struct CThostFtdcSettlementInfoConfirmField			CThostFtdcSettlementInfoConfirmField;
typedef struct CThostFtdcSyncDepositField			CThostFtdcSyncDepositField;
typedef struct CThostFtdcSyncFundMortgageField			CThostFtdcSyncFundMortgageField;
typedef struct CThostFtdcBrokerSyncField			CThostFtdcBrokerSyncField;
typedef struct CThostFtdcSyncingInvestorField			CThostFtdcSyncingInvestorField;
typedef struct CThostFtdcSyncingTradingCodeField			CThostFtdcSyncingTradingCodeField;
typedef struct CThostFtdcSyncingInvestorGroupField			CThostFtdcSyncingInvestorGroupField;
typedef struct CThostFtdcSyncingTradingAccountField			CThostFtdcSyncingTradingAccountField;
typedef struct CThostFtdcSyncingInvestorPositionField			CThostFtdcSyncingInvestorPositionField;
typedef struct CThostFtdcSyncingInstrumentMarginRateField			CThostFtdcSyncingInstrumentMarginRateField;
typedef struct CThostFtdcSyncingInstrumentCommissionRateField			CThostFtdcSyncingInstrumentCommissionRateField;
typedef struct CThostFtdcSyncingInstrumentTradingRightField			CThostFtdcSyncingInstrumentTradingRightField;
typedef struct CThostFtdcQryOrderField			CThostFtdcQryOrderField;
typedef struct CThostFtdcQryTradeField			CThostFtdcQryTradeField;
typedef struct CThostFtdcQryInvestorPositionField			CThostFtdcQryInvestorPositionField;
typedef struct CThostFtdcQryTradingAccountField			CThostFtdcQryTradingAccountField;
typedef struct CThostFtdcQryInvestorField			CThostFtdcQryInvestorField;
typedef struct CThostFtdcQryTradingCodeField			CThostFtdcQryTradingCodeField;
typedef struct CThostFtdcQryInvestorGroupField			CThostFtdcQryInvestorGroupField;
typedef struct CThostFtdcQryInstrumentMarginRateField			CThostFtdcQryInstrumentMarginRateField;
typedef struct CThostFtdcQryInstrumentCommissionRateField			CThostFtdcQryInstrumentCommissionRateField;
typedef struct CThostFtdcQryInstrumentTradingRightField			CThostFtdcQryInstrumentTradingRightField;
typedef struct CThostFtdcQryBrokerField			CThostFtdcQryBrokerField;
typedef struct CThostFtdcQryTraderField			CThostFtdcQryTraderField;
typedef struct CThostFtdcQrySuperUserFunctionField			CThostFtdcQrySuperUserFunctionField;
typedef struct CThostFtdcQryUserSessionField			CThostFtdcQryUserSessionField;
typedef struct CThostFtdcQryPartBrokerField			CThostFtdcQryPartBrokerField;
typedef struct CThostFtdcQryFrontStatusField			CThostFtdcQryFrontStatusField;
typedef struct CThostFtdcQryExchangeOrderField			CThostFtdcQryExchangeOrderField;
typedef struct CThostFtdcQryOrderActionField			CThostFtdcQryOrderActionField;
typedef struct CThostFtdcQryExchangeOrderActionField			CThostFtdcQryExchangeOrderActionField;
typedef struct CThostFtdcQrySuperUserField			CThostFtdcQrySuperUserField;
typedef struct CThostFtdcQryExchangeField			CThostFtdcQryExchangeField;
typedef struct CThostFtdcQryProductField			CThostFtdcQryProductField;
typedef struct CThostFtdcQryInstrumentField			CThostFtdcQryInstrumentField;
typedef struct CThostFtdcQryDepthMarketDataField			CThostFtdcQryDepthMarketDataField;
typedef struct CThostFtdcQryBrokerUserField			CThostFtdcQryBrokerUserField;
typedef struct CThostFtdcQryBrokerUserFunctionField			CThostFtdcQryBrokerUserFunctionField;
typedef struct CThostFtdcQryTraderOfferField			CThostFtdcQryTraderOfferField;
typedef struct CThostFtdcQrySyncDepositField			CThostFtdcQrySyncDepositField;
typedef struct CThostFtdcQrySettlementInfoField			CThostFtdcQrySettlementInfoField;
typedef struct CThostFtdcQryExchangeMarginRateField			CThostFtdcQryExchangeMarginRateField;
typedef struct CThostFtdcQryExchangeMarginRateAdjustField			CThostFtdcQryExchangeMarginRateAdjustField;
typedef struct CThostFtdcQryExchangeRateField			CThostFtdcQryExchangeRateField;
typedef struct CThostFtdcQrySyncFundMortgageField			CThostFtdcQrySyncFundMortgageField;
typedef struct CThostFtdcQryHisOrderField			CThostFtdcQryHisOrderField;
typedef struct CThostFtdcOptionInstrMiniMarginField			CThostFtdcOptionInstrMiniMarginField;
typedef struct CThostFtdcOptionInstrMarginAdjustField			CThostFtdcOptionInstrMarginAdjustField;
typedef struct CThostFtdcOptionInstrCommRateField			CThostFtdcOptionInstrCommRateField;
typedef struct CThostFtdcOptionInstrTradeCostField			CThostFtdcOptionInstrTradeCostField;
typedef struct CThostFtdcQryOptionInstrTradeCostField			CThostFtdcQryOptionInstrTradeCostField;
typedef struct CThostFtdcQryOptionInstrCommRateField			CThostFtdcQryOptionInstrCommRateField;
typedef struct CThostFtdcIndexPriceField			CThostFtdcIndexPriceField;
typedef struct CThostFtdcInputExecOrderField			CThostFtdcInputExecOrderField;
typedef struct CThostFtdcInputExecOrderActionField			CThostFtdcInputExecOrderActionField;
typedef struct CThostFtdcExecOrderField			CThostFtdcExecOrderField;
typedef struct CThostFtdcExecOrderActionField			CThostFtdcExecOrderActionField;
typedef struct CThostFtdcQryExecOrderField			CThostFtdcQryExecOrderField;
typedef struct CThostFtdcExchangeExecOrderField			CThostFtdcExchangeExecOrderField;
typedef struct CThostFtdcQryExchangeExecOrderField			CThostFtdcQryExchangeExecOrderField;
typedef struct CThostFtdcQryExecOrderActionField			CThostFtdcQryExecOrderActionField;
typedef struct CThostFtdcExchangeExecOrderActionField			CThostFtdcExchangeExecOrderActionField;
typedef struct CThostFtdcQryExchangeExecOrderActionField			CThostFtdcQryExchangeExecOrderActionField;
typedef struct CThostFtdcErrExecOrderField			CThostFtdcErrExecOrderField;
typedef struct CThostFtdcQryErrExecOrderField			CThostFtdcQryErrExecOrderField;
typedef struct CThostFtdcErrExecOrderActionField			CThostFtdcErrExecOrderActionField;
typedef struct CThostFtdcQryErrExecOrderActionField			CThostFtdcQryErrExecOrderActionField;
typedef struct CThostFtdcOptionInstrTradingRightField			CThostFtdcOptionInstrTradingRightField;
typedef struct CThostFtdcQryOptionInstrTradingRightField			CThostFtdcQryOptionInstrTradingRightField;
typedef struct CThostFtdcInputForQuoteField			CThostFtdcInputForQuoteField;
typedef struct CThostFtdcForQuoteField			CThostFtdcForQuoteField;
typedef struct CThostFtdcQryForQuoteField			CThostFtdcQryForQuoteField;
typedef struct CThostFtdcExchangeForQuoteField			CThostFtdcExchangeForQuoteField;
typedef struct CThostFtdcQryExchangeForQuoteField			CThostFtdcQryExchangeForQuoteField;
typedef struct CThostFtdcInputQuoteField			CThostFtdcInputQuoteField;
typedef struct CThostFtdcInputQuoteActionField			CThostFtdcInputQuoteActionField;
typedef struct CThostFtdcQuoteField			CThostFtdcQuoteField;
typedef struct CThostFtdcQuoteActionField			CThostFtdcQuoteActionField;
typedef struct CThostFtdcQryQuoteField			CThostFtdcQryQuoteField;
typedef struct CThostFtdcExchangeQuoteField			CThostFtdcExchangeQuoteField;
typedef struct CThostFtdcQryExchangeQuoteField			CThostFtdcQryExchangeQuoteField;
typedef struct CThostFtdcQryQuoteActionField			CThostFtdcQryQuoteActionField;
typedef struct CThostFtdcExchangeQuoteActionField			CThostFtdcExchangeQuoteActionField;
typedef struct CThostFtdcQryExchangeQuoteActionField			CThostFtdcQryExchangeQuoteActionField;
typedef struct CThostFtdcOptionInstrDeltaField			CThostFtdcOptionInstrDeltaField;
typedef struct CThostFtdcForQuoteRspField			CThostFtdcForQuoteRspField;
typedef struct CThostFtdcStrikeOffsetField			CThostFtdcStrikeOffsetField;
typedef struct CThostFtdcQryStrikeOffsetField			CThostFtdcQryStrikeOffsetField;
typedef struct CThostFtdcMarketDataField			CThostFtdcMarketDataField;
typedef struct CThostFtdcMarketDataBaseField			CThostFtdcMarketDataBaseField;
typedef struct CThostFtdcMarketDataStaticField			CThostFtdcMarketDataStaticField;
typedef struct CThostFtdcMarketDataLastMatchField			CThostFtdcMarketDataLastMatchField;
typedef struct CThostFtdcMarketDataBestPriceField			CThostFtdcMarketDataBestPriceField;
typedef struct CThostFtdcMarketDataBid23Field			CThostFtdcMarketDataBid23Field;
typedef struct CThostFtdcMarketDataAsk23Field			CThostFtdcMarketDataAsk23Field;
typedef struct CThostFtdcMarketDataBid45Field			CThostFtdcMarketDataBid45Field;
typedef struct CThostFtdcMarketDataAsk45Field			CThostFtdcMarketDataAsk45Field;
typedef struct CThostFtdcMarketDataUpdateTimeField			CThostFtdcMarketDataUpdateTimeField;
typedef struct CThostFtdcMarketDataExchangeField			CThostFtdcMarketDataExchangeField;
typedef struct CThostFtdcSpecificInstrumentField			CThostFtdcSpecificInstrumentField;
typedef struct CThostFtdcInstrumentStatusField			CThostFtdcInstrumentStatusField;
typedef struct CThostFtdcQryInstrumentStatusField			CThostFtdcQryInstrumentStatusField;
typedef struct CThostFtdcInvestorAccountField			CThostFtdcInvestorAccountField;
typedef struct CThostFtdcPositionProfitAlgorithmField			CThostFtdcPositionProfitAlgorithmField;
typedef struct CThostFtdcDiscountField			CThostFtdcDiscountField;
typedef struct CThostFtdcQryTransferBankField			CThostFtdcQryTransferBankField;
typedef struct CThostFtdcTransferBankField			CThostFtdcTransferBankField;
typedef struct CThostFtdcQryInvestorPositionDetailField			CThostFtdcQryInvestorPositionDetailField;
typedef struct CThostFtdcInvestorPositionDetailField			CThostFtdcInvestorPositionDetailField;
typedef struct CThostFtdcTradingAccountPasswordField			CThostFtdcTradingAccountPasswordField;
typedef struct CThostFtdcMDTraderOfferField			CThostFtdcMDTraderOfferField;
typedef struct CThostFtdcQryMDTraderOfferField			CThostFtdcQryMDTraderOfferField;
typedef struct CThostFtdcQryNoticeField			CThostFtdcQryNoticeField;
typedef struct CThostFtdcNoticeField			CThostFtdcNoticeField;
typedef struct CThostFtdcUserRightField			CThostFtdcUserRightField;
typedef struct CThostFtdcQrySettlementInfoConfirmField			CThostFtdcQrySettlementInfoConfirmField;
typedef struct CThostFtdcLoadSettlementInfoField			CThostFtdcLoadSettlementInfoField;
typedef struct CThostFtdcBrokerWithdrawAlgorithmField			CThostFtdcBrokerWithdrawAlgorithmField;
typedef struct CThostFtdcTradingAccountPasswordUpdateV1Field			CThostFtdcTradingAccountPasswordUpdateV1Field;
typedef struct CThostFtdcTradingAccountPasswordUpdateField			CThostFtdcTradingAccountPasswordUpdateField;
typedef struct CThostFtdcQryCombinationLegField			CThostFtdcQryCombinationLegField;
typedef struct CThostFtdcQrySyncStatusField			CThostFtdcQrySyncStatusField;
typedef struct CThostFtdcCombinationLegField			CThostFtdcCombinationLegField;
typedef struct CThostFtdcSyncStatusField			CThostFtdcSyncStatusField;
typedef struct CThostFtdcQryLinkManField			CThostFtdcQryLinkManField;
typedef struct CThostFtdcLinkManField			CThostFtdcLinkManField;
typedef struct CThostFtdcQryBrokerUserEventField			CThostFtdcQryBrokerUserEventField;
typedef struct CThostFtdcBrokerUserEventField			CThostFtdcBrokerUserEventField;
typedef struct CThostFtdcQryContractBankField			CThostFtdcQryContractBankField;
typedef struct CThostFtdcContractBankField			CThostFtdcContractBankField;
typedef struct CThostFtdcInvestorPositionCombineDetailField			CThostFtdcInvestorPositionCombineDetailField;
typedef struct CThostFtdcParkedOrderField			CThostFtdcParkedOrderField;
typedef struct CThostFtdcParkedOrderActionField			CThostFtdcParkedOrderActionField;
typedef struct CThostFtdcQryParkedOrderField			CThostFtdcQryParkedOrderField;
typedef struct CThostFtdcQryParkedOrderActionField			CThostFtdcQryParkedOrderActionField;
typedef struct CThostFtdcRemoveParkedOrderField			CThostFtdcRemoveParkedOrderField;
typedef struct CThostFtdcRemoveParkedOrderActionField			CThostFtdcRemoveParkedOrderActionField;
typedef struct CThostFtdcInvestorWithdrawAlgorithmField			CThostFtdcInvestorWithdrawAlgorithmField;
typedef struct CThostFtdcQryInvestorPositionCombineDetailField			CThostFtdcQryInvestorPositionCombineDetailField;
typedef struct CThostFtdcMarketDataAveragePriceField			CThostFtdcMarketDataAveragePriceField;
typedef struct CThostFtdcVerifyInvestorPasswordField			CThostFtdcVerifyInvestorPasswordField;
typedef struct CThostFtdcUserIPField			CThostFtdcUserIPField;
typedef struct CThostFtdcTradingNoticeInfoField			CThostFtdcTradingNoticeInfoField;
typedef struct CThostFtdcTradingNoticeField			CThostFtdcTradingNoticeField;
typedef struct CThostFtdcQryTradingNoticeField			CThostFtdcQryTradingNoticeField;
typedef struct CThostFtdcQryErrOrderField			CThostFtdcQryErrOrderField;
typedef struct CThostFtdcErrOrderField			CThostFtdcErrOrderField;
typedef struct CThostFtdcErrorConditionalOrderField			CThostFtdcErrorConditionalOrderField;
typedef struct CThostFtdcQryErrOrderActionField			CThostFtdcQryErrOrderActionField;
typedef struct CThostFtdcErrOrderActionField			CThostFtdcErrOrderActionField;
typedef struct CThostFtdcQryExchangeSequenceField			CThostFtdcQryExchangeSequenceField;
typedef struct CThostFtdcExchangeSequenceField			CThostFtdcExchangeSequenceField;
typedef struct CThostFtdcQueryMaxOrderVolumeWithPriceField			CThostFtdcQueryMaxOrderVolumeWithPriceField;
typedef struct CThostFtdcQryBrokerTradingParamsField			CThostFtdcQryBrokerTradingParamsField;
typedef struct CThostFtdcBrokerTradingParamsField			CThostFtdcBrokerTradingParamsField;
typedef struct CThostFtdcQryBrokerTradingAlgosField			CThostFtdcQryBrokerTradingAlgosField;
typedef struct CThostFtdcBrokerTradingAlgosField			CThostFtdcBrokerTradingAlgosField;
typedef struct CThostFtdcQueryBrokerDepositField			CThostFtdcQueryBrokerDepositField;
typedef struct CThostFtdcBrokerDepositField			CThostFtdcBrokerDepositField;
typedef struct CThostFtdcQryCFMMCBrokerKeyField			CThostFtdcQryCFMMCBrokerKeyField;
typedef struct CThostFtdcCFMMCBrokerKeyField			CThostFtdcCFMMCBrokerKeyField;
typedef struct CThostFtdcCFMMCTradingAccountKeyField			CThostFtdcCFMMCTradingAccountKeyField;
typedef struct CThostFtdcQryCFMMCTradingAccountKeyField			CThostFtdcQryCFMMCTradingAccountKeyField;
typedef struct CThostFtdcBrokerUserOTPParamField			CThostFtdcBrokerUserOTPParamField;
typedef struct CThostFtdcManualSyncBrokerUserOTPField			CThostFtdcManualSyncBrokerUserOTPField;
typedef struct CThostFtdcCommRateModelField			CThostFtdcCommRateModelField;
typedef struct CThostFtdcQryCommRateModelField			CThostFtdcQryCommRateModelField;
typedef struct CThostFtdcMarginModelField			CThostFtdcMarginModelField;
typedef struct CThostFtdcQryMarginModelField			CThostFtdcQryMarginModelField;
typedef struct CThostFtdcEWarrantOffsetField			CThostFtdcEWarrantOffsetField;
typedef struct CThostFtdcQryEWarrantOffsetField			CThostFtdcQryEWarrantOffsetField;
typedef struct CThostFtdcQryInvestorProductGroupMarginField			CThostFtdcQryInvestorProductGroupMarginField;
typedef struct CThostFtdcInvestorProductGroupMarginField			CThostFtdcInvestorProductGroupMarginField;
typedef struct CThostFtdcReqOpenAccountField			CThostFtdcReqOpenAccountField;
typedef struct CThostFtdcReqCancelAccountField			CThostFtdcReqCancelAccountField;
typedef struct CThostFtdcReqChangeAccountField			CThostFtdcReqChangeAccountField;
typedef struct CThostFtdcReqTransferField			CThostFtdcReqTransferField;
typedef struct CThostFtdcRspTransferField			CThostFtdcRspTransferField;
typedef struct CThostFtdcReqRepealField			CThostFtdcReqRepealField;
typedef struct CThostFtdcRspRepealField			CThostFtdcRspRepealField;
typedef struct CThostFtdcReqQueryAccountField			CThostFtdcReqQueryAccountField;
typedef struct CThostFtdcRspQueryAccountField			CThostFtdcRspQueryAccountField;
typedef struct CThostFtdcFutureSignIOField			CThostFtdcFutureSignIOField;
typedef struct CThostFtdcRspFutureSignInField			CThostFtdcRspFutureSignInField;
typedef struct CThostFtdcReqFutureSignOutField			CThostFtdcReqFutureSignOutField;
typedef struct CThostFtdcRspFutureSignOutField			CThostFtdcRspFutureSignOutField;
typedef struct CThostFtdcReqQueryTradeResultBySerialField			CThostFtdcReqQueryTradeResultBySerialField;
typedef struct CThostFtdcRspQueryTradeResultBySerialField			CThostFtdcRspQueryTradeResultBySerialField;
typedef struct CThostFtdcReqDayEndFileReadyField			CThostFtdcReqDayEndFileReadyField;
typedef struct CThostFtdcReturnResultField			CThostFtdcReturnResultField;
typedef struct CThostFtdcVerifyFuturePasswordField			CThostFtdcVerifyFuturePasswordField;
typedef struct CThostFtdcVerifyCustInfoField			CThostFtdcVerifyCustInfoField;
typedef struct CThostFtdcVerifyFuturePasswordAndCustInfoField			CThostFtdcVerifyFuturePasswordAndCustInfoField;
typedef struct CThostFtdcDepositResultInformField			CThostFtdcDepositResultInformField;
typedef struct CThostFtdcReqSyncKeyField			CThostFtdcReqSyncKeyField;
typedef struct CThostFtdcRspSyncKeyField			CThostFtdcRspSyncKeyField;
typedef struct CThostFtdcNotifyQueryAccountField			CThostFtdcNotifyQueryAccountField;
typedef struct CThostFtdcTransferSerialField			CThostFtdcTransferSerialField;
typedef struct CThostFtdcQryTransferSerialField			CThostFtdcQryTransferSerialField;
typedef struct CThostFtdcNotifyFutureSignInField			CThostFtdcNotifyFutureSignInField;
typedef struct CThostFtdcNotifyFutureSignOutField			CThostFtdcNotifyFutureSignOutField;
typedef struct CThostFtdcNotifySyncKeyField			CThostFtdcNotifySyncKeyField;
typedef struct CThostFtdcQryAccountregisterField			CThostFtdcQryAccountregisterField;
typedef struct CThostFtdcAccountregisterField			CThostFtdcAccountregisterField;
typedef struct CThostFtdcOpenAccountField			CThostFtdcOpenAccountField;
typedef struct CThostFtdcCancelAccountField			CThostFtdcCancelAccountField;
typedef struct CThostFtdcChangeAccountField			CThostFtdcChangeAccountField;
typedef struct CThostFtdcSecAgentACIDMapField			CThostFtdcSecAgentACIDMapField;
typedef struct CThostFtdcQrySecAgentACIDMapField			CThostFtdcQrySecAgentACIDMapField;
typedef struct CThostFtdcUserRightsAssignField			CThostFtdcUserRightsAssignField;
typedef struct CThostFtdcBrokerUserRightAssignField			CThostFtdcBrokerUserRightAssignField;
typedef struct CThostFtdcDRTransferField			CThostFtdcDRTransferField;
typedef struct CThostFtdcFensUserInfoField			CThostFtdcFensUserInfoField;
typedef struct CThostFtdcCurrTransferIdentityField			CThostFtdcCurrTransferIdentityField;
typedef struct CThostFtdcLoginForbiddenUserField			CThostFtdcLoginForbiddenUserField;
typedef struct CThostFtdcQryLoginForbiddenUserField			CThostFtdcQryLoginForbiddenUserField;
typedef struct CThostFtdcMulticastGroupInfoField			CThostFtdcMulticastGroupInfoField;
typedef struct CThostFtdcTradingAccountReserveField			CThostFtdcTradingAccountReserveField;

/********************************************************************************************************/
/********************For Market Data (related to ThostFtdcMdApi.h)***************************************/
/********************************************************************************************************/
/*2 functions, MD object, create & free.*/
//void *MD_create(char *flowpath,char *servername, char *brokerid, char *inverstorid, char *password);
void* MD_create(char *flowpath, char *servername, char *bid, char *iid, char *pd, char **InstrumentIDs, int InstrumentNum);
void MD_free(void* md);

/*In class CThostFtdcMdSpi, there are 12 callback functions.*/
// related to "virtual void OnFrontConnected(){};"
typedef void(* fnOnFrontConnected)(void* md); 
void MD_RegOnFrontConnected(void* md, fnOnFrontConnected pCallback);
// related to "virtual void OnFrontDisconnected(int nReason){};"
typedef void(* fnOnFrontDisconnected)(void* md, int nReason);
void MD_RegOnFrontDisconnected(void* md, fnOnFrontDisconnected pCallback);
// related to "virtual void OnHeartBeatWarning(int nTimeLapse){};"
typedef void(* fnOnHeartBeatWarning)(void* md, int nTimeLapse);
void MD_RegOnHeartBeatWarning(void* md, fnOnHeartBeatWarning pCallback);
// related to "virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspUserLogin)(void *md, CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspUserLogin(void *md, fnOnRspUserLogin pCallback);
// related to "virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspUserLogout)(void *md, CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspUserLogout(void *md, fnOnRspUserLogout pCallback);
// related to "virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspError)(void* md, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspError(void* md, fnOnRspError pCallback);
// related to "virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspSubMarketData)(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspSubMarketData(void *md, fnOnRspSubMarketData pCallback);
// related to "virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspUnSubMarketData)(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspUnSubMarketData(void *md, fnOnRspUnSubMarketData pCallback);
// related to "virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspSubForQuoteRsp)(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspSubForQuoteRsp(void *md, fnOnRspSubForQuoteRsp pCallback);
// related to "virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};"
typedef void(* fnOnRspUnSubForQuoteRsp)(void *md, CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, /*bool*/ int bIsLast);
void MD_RegOnRspUnSubForQuoteRsp(void *md, fnOnRspUnSubForQuoteRsp pCallback);
// related to "virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {};"
typedef void(* fnOnRtnDepthMarketData)(void* md, CThostFtdcDepthMarketDataField *pDepthMarketData);
void MD_RegOnRtnDepthMarketData(void* md, fnOnRtnDepthMarketData pCallback);
// related to "virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {};"
typedef void(* fnOnRtnForQuoteRsp)(void* md, CThostFtdcForQuoteRspField *pForQuoteRsp);
void MD_RegOnRtnForQuoteRsp(void* md, fnOnRtnForQuoteRsp pCallback);

/*In class CThostFtdcMdApi, there are 13 api functions.*/
void MD_init(void *md);
void MD_join(void *md);
void MD_getTradingDay(void *md);
void MD_registerFront(void *md, char *pszFrontAddress);
void MD_registerNameServer(void *md, char *pszNsAddress);
void MD_registerFensUserInfo(void *md, CThostFtdcFensUserInfoField * pFensUserInfo);
void MD_registerSpi(void *md);
int MD_subscribeMarketData(void *md, char *ppInstrumentID[], int nCount);
int MD_unSubscribeMarketData(void *md, char *ppInstrumentID[], int nCount);
int MD_subscribeForQuoteRsp(void *md, char *ppInstrumentID[], int nCount);
int MD_unSubscribeForQuoteRsp(void *md, char *ppInstrumentID[], int nCount);
int MD_reqUserLogin(void *md);
int MD_reqUserLogout(void *md);

CThostFtdcDepthMarketDataField *MD_getOneDMDmsg(void *md, double *arrivetime);

#ifdef __cplusplus
}
#endif

#endif//end of CTP_C_API_CTPCAPI_H
