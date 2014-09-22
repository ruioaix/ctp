#include "threadfunc.h"

#include "ctpcapi.h"
#include "mongoapi.h"
#include "verbose.h"
#include "safe.h"
#include <string.h>

static void getupdatetime(char *updatetime, int *hour, int *minute, int *second) {
	char tt[3];
	tt[2] = '\0';
	tt[0] = updatetime[0];
	tt[1] = updatetime[1];
	*hour = strtol(tt, NULL, 10);
	tt[0] = updatetime[3];
	tt[1] = updatetime[4];
	*minute = strtol(tt, NULL, 10);
	tt[0] = updatetime[6];
	tt[1] = updatetime[7];
	*second = strtol(tt, NULL, 10);
}

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
			getupdatetime(pDepthMarketData->UpdateTime, &hour, &minute, &second);
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
			getupdatetime(pDepthMarketData->UpdateTime, &hour, &minute, &second);

			if ( ((hour >  9 || (hour== 9 && minute>=14)) &&
				  (hour < 11 || (hour==11 && minute<30) || (hour==11 && minute==30 && second==0))) ||
				 ((hour >= 13 ) &&
				  (hour < 15 || (hour==15 && minute<15) || (hour==15 && minute==15 && second==0)))
			   ){
				TD_reqOrderInsert(td, -1, pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, 1, 0, THOST_FTDC_OPT_AnyPrice, THOST_FTDC_TC_GFD,THOST_FTDC_CC_Immediately,0,THOST_FTDC_VC_AV); 
			}
			else {
				TD_reqOrderInsert(td, -1, pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, 1, 0, THOST_FTDC_OPT_AnyPrice, THOST_FTDC_TC_GFD,THOST_FTDC_CC_Immediately,0,THOST_FTDC_VC_AV); 
			}
		}
		usleep(2000);
	}
	return NULL;
}
