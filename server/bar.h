#ifndef CTP_C_API_SERVER_BAR_H
#define CTP_C_API_SERVER_BAR_H

#define BAR_DAYSNUM_MAX 8192
#define BAR_METADATA_1DNUM_MAX 32500
#define BARNUM_1MIN1DAY 273
#define BARNUM_1MIN1DAY_HALF 135

//one day bar element.
struct BARELEMENT {
	int isComplete;
	int YMD;
	int *btimeHMS;
	int *etimeHMS;
	double *openPrice;
	double *closePrice;
	double *uplimitPrice;
	double *lowlimitPrice;
	int *volume;
};

//one day
struct BAR {
	char InstrumentID[16];
	int barLen;
	int num;
	int head;
	int tail;
	struct BARELEMENT *bars[BAR_DAYSNUM_MAX];
};

#include <mongoc.h>
struct BAR *create_1MTYPE_BAR_from_MongoDB(mongoc_collection_t *cll, int beginYMD, int endYMD);

#endif
