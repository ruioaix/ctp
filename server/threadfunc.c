#include "threadfunc.h"

#include "ctpcapi.h"
#include "mongoapi.h"
#include "verbose.h"
#include "safe.h"
#include "ctphelp.h"
#include <string.h>

void *DMDMSG_insertIntoMongoDB(void *ThreadIM) {
	struct ThreadIM *mim = ThreadIM;

	void *md = mim->md;
	mongoc_client_t *client = mim->client;
	mongoc_collection_t **mcollections = mim->mcollections;
	int mcollectionsNum = mim->mcollectionsNum;
	char **InstrumentIDs = mim->InstrumentIDs;

	while (*(mim->running)) {
		CThostFtdcDepthMarketDataField *pDepthMarketData = MD_2_getOneDMDmsg(md);
		if (pDepthMarketData != NULL) {
			printtlb("%s: updated time: %s, update mill time : %4d", pDepthMarketData->InstrumentID, pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec);
			int hour, minute, second;
			CTPHELP_updatetime2HMS(pDepthMarketData->UpdateTime, &hour, &minute, &second);
			if ( ((hour >  9 || (hour== 9 && minute>=14)) &&
				  (hour < 11 || (hour==11 && minute<30) || (hour==11 && minute==30 && second==0))) ||
				 ((hour >= 13 ) &&
				  (hour < 15 || (hour==15 && minute<15) || (hour==15 && minute==15 && second==0)))
			   ){
				int i;
				for (i = 0; i < mcollectionsNum; ++i) {
					if (strcmp(pDepthMarketData->InstrumentID, InstrumentIDs[i]) == 0) {
						break;
					}
				}
				MongoAPI_insert_DMD(client, mcollections[i], pDepthMarketData);
			}
			else {
				MongoAPI_insert_DMD(client, mcollections[mcollectionsNum], pDepthMarketData);
			}
		}
		else {
			usleep(200000);
		}
	}
	return NULL;
}

static void getcurrentdate(int *year, int *month, int *day, int *hour, int *minute, int *second) {
	time_t t = time(NULL);
	struct tm *tm = gmtime(&t);
	*year = tm->tm_year-100;
	*month = tm->tm_mon+1;
	*day = tm->tm_mday;
	*hour = tm->tm_hour;
	*minute = tm->tm_min;
	*second = tm->tm_sec;
}

static void get_next_next_month(int year, int month, int *nnyear, int *nnmonth) {
	if (month<11) {
		*nnyear = year;
		*nnmonth = month+2;
	}
	else {
		*nnyear = year+1;
		*nnmonth = (month+2)%12;
	}
}

void *INSTRMENT_revise(void *ThreadIM) {
	struct ThreadIM *mim = ThreadIM;
	void *md = mim->md;
	int mcollectionsNum = mim->mcollectionsNum;
	char **InstrumentIDs = mim->InstrumentIDs;
	int _year, _month, _day, _hour, _minute, _second;
	getcurrentdate(&_year, &_month, &_day, &_hour, &_minute, &_second);
	while (*(mim->running)) {
		sleep(5);
		int year, month, day, hour, minute, second;
		getcurrentdate(&year, &month, &day, &hour, &minute, &second);
		if (day == 1 && hour == 12) {
			if (_month != month) {
				int nnyear, nnmonth;
				get_next_next_month(year, month, &nnyear, &nnmonth);
				char insname_old[10];
				char insname_new[10];
				sprintf(insname_old, "IF%02d%02d", _year, _month);
				sprintf(insname_new, "IF%02d%02d", nnyear, nnmonth);
				int i;
				for (i = 0; i < mcollectionsNum; ++i) {
					if (strcmp(insname_old, InstrumentIDs[i]) == 0) {
						free(InstrumentIDs[i]);
						InstrumentIDs[i] = smalloc(strlen(insname_new)+1);
						memcpy(InstrumentIDs[i], insname_new, strlen(insname_new));
						InstrumentIDs[i][strlen(insname_new)] = '\0';
					}
				}
				MD_subscribeMarketData(md, InstrumentIDs, mcollectionsNum); 
				_year=year;
				_month=month;
				_day = day;
				_hour = hour;
				_minute = minute;
				_second = second;
			}
		}
	}
	return NULL;
}

void *EVENT_500ms_dmdmsg(void *ThreadIM) {
	struct ThreadIM *mim = ThreadIM;

	void *md = mim->md;
	void *td = mim->td;

	while (!TD_isready(td)) {
		sleep(1);
	}

	while (*(mim->running)) {
		long ts;
		long tus;
		int size;
		CThostFtdcDepthMarketDataField *pDepthMarketData = MD_getOneDMDmsg(md, &ts, &tus, &size);
		if (pDepthMarketData != NULL) {
			struct timespec tv;
			clock_gettime(CLOCK_REALTIME, &tv);

			printtlc("%s: updated time: %s, update mill time : %4d, arrive: %ld.%09ld, delay: %.10f, valid size: %3d",\
				   	pDepthMarketData->InstrumentID, \
					pDepthMarketData->UpdateTime, \
					pDepthMarketData->UpdateMillisec, \
					ts, tus, tv.tv_sec-ts+(tv.tv_nsec-tus)*1E-9, size);

			int hour, minute, second;
			CTPHELP_updatetime2HMS(pDepthMarketData->UpdateTime, &hour, &minute, &second);

			if ( ((hour >  9 || (hour== 9 && minute>=14)) &&
				  (hour < 11 || (hour==11 && minute<30) || (hour==11 && minute==30 && second==0))) ||
				 ((hour >= 13 ) &&
				  (hour < 15 || (hour==15 && minute<15) || (hour==15 && minute==15 && second==0)))
			   ){
				//TD_reqOrderInsert(td, -1, pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, 0, 1, pDepthMarketData->BidPrice1, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD,THOST_FTDC_VC_AV); 
			}
			else {
				//TD_GetApi(td)->ReqOrderInsert(OrderRef, InstrumentID, Direction, CombOffsetFlag, VolumeTotalOriginal, LimitPrice, OrderPriceType, TimeCondition, ContingentCondition, StopPrice, VolumeCondition);
				//TD_reqOrderInsert(td, -1, pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, 0, 1, pDepthMarketData->BidPrice1, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD,THOST_FTDC_CC_Immediately,0,THOST_FTDC_VC_AV); 
				//TD_reqOrderInsert_ljsj(td, pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, 0, 3);
				//TD_reqOrderInsert_ljxj(td, pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, 0, 3, pDepthMarketData->AskPrice1);
				static int k = 20;
				//TD_reqOrderInsert(td, k, pDepthMarketData->InstrumentID, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_OF_Open, THOST_FTDC_D_Buy, k-3, pDepthMarketData->AskPrice1, THOST_FTDC_TC_GFD, THOST_FTDC_VC_AV); 
				//usleep(50000);
				//TD_limitOrder_open_buy(td, k, pDepthMarketData->InstrumentID, pDepthMarketData->AskVolume1, pDepthMarketData->AskPrice1, THOST_FTDC_TC_GFD, THOST_FTDC_VC_AV);
				//sleep(5);
				//TD_limitOrder_sell_closeToday(td, k+1, pDepthMarketData->InstrumentID, pDepthMarketData->AskVolume1, pDepthMarketData->AskPrice1, THOST_FTDC_TC_GFD, THOST_FTDC_VC_AV);

				TD_limitOrder_open_sell(td, k, pDepthMarketData->InstrumentID, pDepthMarketData->BidVolume1, pDepthMarketData->BidPrice1, THOST_FTDC_TC_GFD, THOST_FTDC_VC_AV);
				sleep(5);
				TD_limitOrder_buy_closeToday(td, k+1, pDepthMarketData->InstrumentID, pDepthMarketData->BidVolume1, pDepthMarketData->BidPrice1, THOST_FTDC_TC_GFD, THOST_FTDC_VC_AV);
				++k;
				if (k == 22) {
				return NULL;
				}
				return NULL;
			}
		}
		usleep(2000);
	}
	return NULL;
}
