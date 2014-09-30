#ifndef CTP_C_API_SERVER_METRICS_H
#define CTP_C_API_SERVER_METRICS_H

#include "bar.h"

struct EMABAR {
	struct BAR *bar;
	double *longEMA[BAR_DAYSNUM_MAX];
	double *mediEMA[BAR_DAYSNUM_MAX];
	double *shotEMA[BAR_DAYSNUM_MAX];
	int longNum;
	int mediNum;
	int shotNum;
};

struct EMABAR *create_EMABAR(struct BAR *bar, int longnum, int medinum, int shotnum);
void EMABAR_syn(struct EMABAR *eb, int index);

#endif
