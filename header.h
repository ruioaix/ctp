// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� QUANTBOXC2CTP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
//  ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef CTP_C_API_HEADER_H
#define CTP_C_API_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

#define bool int
#include "cpp_api/ThostFtdcUserApiStruct.h"

//need for c
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

//���ڷָ�����ĺ��б���ǰ�û���ַ�б����Բ��ܳ��֡�:��һ���
#define _QUANTBOXC2CTP_SEPS_ ",;"

//����״̬ö��
enum ConnectionStatus
{
	E_uninit,		//δ��ʼ��
	E_inited,		//�Ѿ���ʼ��
	E_unconnected,	//�����Ѿ��Ͽ�
	E_connecting,	//������
	E_connected,	//���ӳɹ�
	E_authing,		//��Ȩ��
	E_authed,		//��Ȩ�ɹ�
	E_logining,		//��¼��
	E_logined,		//��¼�ɹ�
	E_confirming,	//���㵥ȷ����
	E_confirmed,	//�Ѿ�ȷ��
	E_conn_max		//���ֵ
};

typedef enum ConnectionStatus ConnectionStatus;
typedef enum THOST_TE_RESUME_TYPE THOST_TE_RESUME_TYPE;

//�ص��������Ͷ��壨Ϊ��д���㣬����ĸ����
typedef void( *fnOnConnect)(void* pApi, CThostFtdcRspUserLoginField *pRspUserLogin, ConnectionStatus result);//���Ӻ�Ľ��״̬
typedef void( *fnOnDisconnect)(void* pApi, CThostFtdcRspInfoField *pRspInfo, ConnectionStatus step);//����ʱ������״̬
typedef void( *fnOnErrRtnOrderAction)(void* pTraderApi, CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);
typedef void( *fnOnErrRtnOrderInsert)(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
typedef void( *fnOnErrRtnQuoteAction)(void* pTraderApi, CThostFtdcQuoteActionField *pQuoteAction, CThostFtdcRspInfoField *pRspInfo);
typedef void( *fnOnErrRtnQuoteInsert)(void* pTraderApi, CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo);
typedef void( *fnOnRspError)(void* pApi, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspOrderAction)(void* pTraderApi, CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspOrderInsert)(void* pTraderApi, CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryDepthMarketData)(void* pTraderApi, CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryInstrument)(void* pTraderApi, CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryInstrumentCommissionRate)(void* pTraderApi, CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryInstrumentMarginRate)(void* pTraderApi, CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryInvestorPosition)(void* pTraderApi, CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryInvestorPositionDetail)(void* pTraderApi, CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryOrder)(void* pTraderApi, CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryTrade)(void* pTraderApi, CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQrySettlementInfo)(void* pTraderApi, CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQryTradingAccount)(void* pTraderApi, CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQuoteAction)(void* pTraderApi, CThostFtdcInputQuoteActionField *pInputQuoteAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRspQuoteInsert)(void* pTraderApi, CThostFtdcInputQuoteField *pInputQuote, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void( *fnOnRtnDepthMarketData)(void* pMdUserApi, CThostFtdcDepthMarketDataField *pDepthMarketData);
typedef void( *fnOnRtnForQuoteRsp)(void* pMdUserApi, CThostFtdcForQuoteRspField *pForQuoteRsp);
typedef void( *fnOnRtnInstrumentStatus)(void* pTraderApi, CThostFtdcInstrumentStatusField *pInstrumentStatus);
typedef void( *fnOnRtnOrder)(void* pTraderApi, CThostFtdcOrderField *pOrder);
typedef void( *fnOnRtnQuote)(void* pTraderApi, CThostFtdcQuoteField *pQuote);
typedef void( *fnOnRtnTrade)(void* pTraderApi, CThostFtdcTradeField *pTrade);


//����������Ϣ���У�֧����Ӧ����ͽ���
void*  CTP_CreateMsgQueue();

//����Ϣ����ע��ص�����
void  CTP_RegOnConnect(void* pMsgQueue, fnOnConnect pCallback);
void  CTP_RegOnDisconnect(void* pMsgQueue, fnOnDisconnect pCallback);
void  CTP_RegOnErrRtnOrderAction(void* pMsgQueue, fnOnErrRtnOrderAction pCallback);
void  CTP_RegOnErrRtnOrderInsert(void* pMsgQueue, fnOnErrRtnOrderInsert pCallback);
void  CTP_RegOnErrRtnQuoteAction(void* pMsgQueue, fnOnErrRtnQuoteAction pCallback);
void  CTP_RegOnErrRtnQuoteInsert(void* pMsgQueue, fnOnErrRtnQuoteInsert pCallback);
void  CTP_RegOnRspError(void* pMsgQueue, fnOnRspError pCallback);
void  CTP_RegOnRspOrderAction(void* pMsgQueue, fnOnRspOrderAction pCallback);
void  CTP_RegOnRspOrderInsert(void* pMsgQueue, fnOnRspOrderInsert pCallback);
void  CTP_RegOnRspQuoteAction(void* pMsgQueue, fnOnRspQuoteAction pCallback);
void  CTP_RegOnRspQuoteInsert(void* pMsgQueue, fnOnRspQuoteInsert pCallback);
void  CTP_RegOnRspQryDepthMarketData(void* pMsgQueue, fnOnRspQryDepthMarketData pCallback);
void  CTP_RegOnRspQryInstrument(void* pMsgQueue, fnOnRspQryInstrument pCallback);
void  CTP_RegOnRspQryInstrumentCommissionRate(void* pMsgQueue, fnOnRspQryInstrumentCommissionRate pCallback);
void  CTP_RegOnRspQryInstrumentMarginRate(void* pMsgQueue, fnOnRspQryInstrumentMarginRate pCallback);
void  CTP_RegOnRspQryInvestorPosition(void* pMsgQueue, fnOnRspQryInvestorPosition pCallback);
void  CTP_RegOnRspQryInvestorPositionDetail(void* pMsgQueue, fnOnRspQryInvestorPositionDetail pCallback);
void  CTP_RegOnRspQryOrder(void* pMsgQueue, fnOnRspQryOrder pCallback);
void  CTP_RegOnRspQryTrade(void* pMsgQueue, fnOnRspQryTrade pCallback);
void  CTP_RegOnRspQrySettlementInfo(void* pMsgQueue, fnOnRspQrySettlementInfo pCallback);
void  CTP_RegOnRspQryTradingAccount(void* pMsgQueue, fnOnRspQryTradingAccount pCallback);
void  CTP_RegOnRtnDepthMarketData(void* pMsgQueue, fnOnRtnDepthMarketData pCallback);
void  CTP_RegOnRtnForQuoteRsp(void* pMsgQueue, fnOnRtnForQuoteRsp pCallback);
void  CTP_RegOnRtnInstrumentStatus(void* pMsgQueue, fnOnRtnInstrumentStatus pCallback);
void  CTP_RegOnRtnOrder(void* pMsgQueue, fnOnRtnOrder pCallback);
void  CTP_RegOnRtnQuote(void* pMsgQueue, fnOnRtnQuote pCallback);
void  CTP_RegOnRtnTrade(void* pMsgQueue, fnOnRtnTrade pCallback);

//�ͷ���Ϣ����
void  CTP_ReleaseMsgQueue(void* pMsgQueue);
//�����Ϣ��������
void  CTP_ClearMsgQueue(void* pMsgQueue);

//������Ϣ���е�һ����¼
bool  CTP_ProcessMsgQueue(void* pMsgQueue);
//�������лص����������߳�
void  CTP_StartMsgQueue(void* pMsgQueue);
//ֹͣ���лص����������߳�
void  CTP_StopMsgQueue(void* pMsgQueue);


//����ӿ�=======================================

//��������ӿ�
void*  MD_CreateMdApi();
//����Ϣ����ע�ᵽ����ӿ���
void  MD_RegMsgQueue2MdApi(void* pMdUserApi, void* pMsgQueue);
//����
void  MD_Connect(
		void* pMdUserApi,
		const char* szPath,
		const char* szAddresses,
		const char* szBrokerId,
		const char* szInvestorId,
		const char* szPassword);

//���ĺ�Լ�������Լ�ԡ�,���ָ�����֤ȯͳһ���ýӿڣ�����������Ŀǰ��Ч
void  MD_Subscribe(void* pMdUserApi, const char* szInstrumentIDs, const char* szExchageID);
//ȡ�����ģ������Լ�ԡ�,���ָ�����֤ȯͳһ���ýӿڣ�����������Ŀǰ��Ч
void  MD_Unsubscribe(void* pMdUserApi, const char* szInstrumentIDs, const char* szExchageID);
//���ĺ�Լ�������Լ�ԡ�,���ָ�����֤ȯͳһ���ýӿڣ�����������Ŀǰ��Ч
void  MD_SubscribeQuote(void* pMdUserApi, const char* szInstrumentIDs, const char* szExchageID);
//ȡ�����ģ������Լ�ԡ�,���ָ�����֤ȯͳһ���ýӿڣ�����������Ŀǰ��Ч
void  MD_UnsubscribeQuote(void* pMdUserApi, const char* szInstrumentIDs, const char* szExchageID);
//�Ͽ�����
void  MD_Disconnect(void* pMdUserApi);
//�ͷ�����ӿ�
void  MD_ReleaseMdApi(void* pMdUserApi);

//���׽ӿ�=======================================
void*  TD_CreateTdApi();
//����Ϣ����ע�ᵽ���׽ӿ���
void  TD_RegMsgQueue2TdApi(void* pTraderApi, void* pMsgQueue);
//����
void  TD_Connect(
		void* pTraderApi,
		const char* szPath,
		const char* szAddresses,
		const char* szBrokerId,
		const char* szInvestorId,
		const char* szPassword,
		THOST_TE_RESUME_TYPE nResumeType,
		const char* szUserProductInfo,
		const char* szAuthCode);

//����
int  TD_SendOrder(
		void* pTraderApi,
		int OrderRef,
		const char* szInstrument,
		const char* szExchange,
		TThostFtdcDirectionType Direction,
		const char* szCombOffsetFlag,
		const char* szCombHedgeFlag,
		TThostFtdcVolumeType VolumeTotalOriginal,
		double LimitPrice,
		TThostFtdcOrderPriceTypeType OrderPriceType,
		TThostFtdcTimeConditionType TimeCondition,
		TThostFtdcContingentConditionType ContingentCondition,
		double StopPrice,
		TThostFtdcVolumeConditionType VolumeCondition);

//����
void  TD_CancelOrder(void* pTraderApi, CThostFtdcOrderField *pOrder);

//����
int  TD_SendQuote(
		void* pTraderApi,
		int QuoteRef,
		const char* szInstrument,
		const char* szExchange,
		double	AskPrice,
		double	BidPrice,
		int AskVolume,
		int BidVolume,
		TThostFtdcOffsetFlagType AskOffsetFlag,
		TThostFtdcOffsetFlagType BidOffsetFlag,
		TThostFtdcHedgeFlagType	AskHedgeFlag,
		TThostFtdcHedgeFlagType	BidHedgeFlag);

//����
void  TD_CancelQuote(void* pTraderApi, CThostFtdcQuoteField *pQuote);

//�Ͽ�����
void  TD_Disconnect(void* pTraderApi);
//�ͷ�����ӿ�
void  TD_ReleaseTdApi(void* pTraderApi);
//���ۺϳֲ�
void  TD_ReqQryInvestorPosition(void* pTraderApi, const char* szInstrumentId);
//��ֲ���ϸ
void  TD_ReqQryInvestorPositionDetail(void* pTraderApi, const char* szInstrumentId);
//���ʽ��˺�
void  TD_ReqQryTradingAccount(void* pTraderApi);
//���Լ
void  TD_ReqQryInstrument(void* pTraderApi, const char* szInstrumentId);
//��������
void  TD_ReqQryInstrumentCommissionRate(void* pTraderApi, const char* szInstrumentId);
//�鱣֤��
void  TD_ReqQryInstrumentMarginRate(void* pTraderApi, const char* szInstrumentId, TThostFtdcHedgeFlagType HedgeFlag);
//���������
void  TD_ReqQryDepthMarketData(void* pTraderApi, const char* szInstrumentId);
//�����ѯͶ���߽�����
void  TD_ReqQrySettlementInfo(void* pTraderApi, const char* szTradingDay);

#ifdef __cplusplus
}
#endif


#endif//end of _QUANTBOXC2CTP_H_
