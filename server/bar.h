#ifndef CTP_C_API_SERVER_BAR_H
#define CTP_C_API_SERVER_BAR_H

#define BAR_DAYSNUM_MAX 8192

enum BARTYPE {
	BARTYPE_1M= 1,
	BARTYPE_5M = 5,
	BARTYPE_10M = 10,
	BARTYPE_20M = 20,
	BARTYPE_1H = 60,
	BARTYPE_1D = 1440,
};

enum BAR1DNUM {
	BAR1DNUM_1M = 273,
	BAR1DNUM_5M = 54,
	BAR1DNUM_10M = 28,
	BAR1DNUM_20M = 14,
	BAR1DNUM_1H =	6,
	BAR1DNUM_1D = 1,
};

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

struct BAR {
	char InstrumentID[16];
	enum BARTYPE type;
	enum BAR1DNUM num;
	int head;
	int tail;
	struct BARELEMENT *bars[BAR_DAYSNUM_MAX];
};

#include <mongoc.h>
struct BAR *create_1MTYPE_BAR_from_MongoDB(mongoc_collection_t *cll, int beginYMD, int endYMD);

#endif
