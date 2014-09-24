#ifndef CTP_C_API_SERVER_BAR_H
#define CTP_C_API_SERVER_BAR_H

#define BARSNUM_ONE_MINUTE_HALFDAY 135
#define BAR_METADATA_MAX_NUM_ONEDAY 32500

struct BAR_METADATA {
	int num;
	int *ymd;
	int *hour; 
	int *minute; 
	int *second; 
	int *millsecond;
	int *volume;
	double *lastprice;
};

enum BAR_TYPE {
	BAR_ONE_M,
	BAR_FIVE_M,
	BAR_TEN_M,
	BAR_TWTENTY_M,
	BAR_ONE_H,
	BAR_ONE_D
};
struct BAR {
	enum BAR_TYPE type;
	int barsNum;
	char InstrumentID[10];
	int beginYMD;
	int *YMD;
	int *btimeHMS;
	int *etimeHMS;
	double *openPrice;
	double *closePrice;
	double *uplimitPrice;
	double *lowlimitPrice;
	int *volume;
};

int BAR_index(struct BAR *bar, int ymd, int hour, int minute);
void BAR_ymd_hms(struct BAR *bar, int index, int *ymd, int *hms);

#include <mongoc.h>
struct BAR *create_BAR_F_MongoDB(mongoc_collection_t *cll, enum BAR_TYPE type, int beginYMD, int endYMD);

#endif
