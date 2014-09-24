#ifndef CTP_C_API_SERVER_BAR_H
#define CTP_C_API_SERVER_BAR_H

#define BAR_METADATA_MAX_NUM_ONEDAY 32500

enum BAR_TYPE {
	BAR_ONE_M = 1,
	BAR_FIVE_M = 5,
	BAR_TEN_M = 10,
	BAR_TWTENTY_M = 20,
	BAR_ONE_H = 60,
	BAR_ONE_D = 1440,
};

enum BAR_NUM_ONE_DAY {
	BAR_NUM_ONE_M = 273,
	BAR_NUM_FIVE_M = 54,
	BAR_NUM_TEN_M = 28,
	BAR_NUM_TWTENTY_M = 14,
	BAR_NUM_ONE_H =	6,
	BAR_NUM_ONE_D = 1,
};

//one day bar element.
struct BAR_ELEMENT {
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

struct BAR {
	enum BAR_TYPE type;
	enum BAR_NUM_ONE_DAY barsNUM;
	char InstrumentID[16];
	int head;
	int tail;
	struct BAR_ELEMENT *bar[8192];
};

void BAR_index(struct BAR *bar, int ymd, int hour, int minute, int *i1, int *i2);
void BAR_ymd_hms(struct BAR *bar, int i1, int i2, int *ymd, int *hms);

#include <mongoc.h>
struct BAR *create_BAR_F_MongoDB(mongoc_collection_t *cll, enum BAR_TYPE type, int beginYMD, int endYMD);

#endif
