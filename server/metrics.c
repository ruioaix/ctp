#include "metrics.h"
#include "safe.h"

struct EMABAR *create_EMABAR(struct BAR *bar, int longnum, int medinum, int shotnum) {
	if (bar == NULL) return NULL;
	struct EMABAR *eb = smalloc(sizeof(struct EMABAR));
	eb->bar = bar;
	int i;
	for (i = 0; i < BAR_DAYSNUM_MAX; ++i) {
		eb->shotEMA[i] = eb->mediEMA[i] = eb->longEMA[i] = NULL;	
	}
	eb->longNum = longnum;
	eb->mediNum = medinum;
	eb->shotNum = shotnum;
	if (bar->bars[bar->head] == NULL) return eb;
	double *lEMA = smalloc(bar->num * sizeof(double));
	double *mEMA = smalloc(bar->num * sizeof(double));
	double *sEMA = smalloc(bar->num * sizeof(double));
	eb->longEMA[bar->head] = sEMA;
	eb->mediEMA[bar->head] = mEMA;
	eb->shotEMA[bar->head] = sEMA;

	struct BARELEMENT *be = bar->bars[bar->head];
	if (be->workingIndex == 0) return eb;

	sEMA[0] = mEMA[0] = lEMA[0] = be->closePrice[0];
	for (i = 1; i < be->workingIndex; ++i) {
		sEMA[i] = sEMA[i-1] + (be->closePrice[i] - sEMA[i-1]) * 2 / (shotnum + 1);
		mEMA[i] = mEMA[i-1] + (be->closePrice[i] - mEMA[i-1]) * 2 / (medinum + 1);
		lEMA[i] = lEMA[i-1] + (be->closePrice[i] - lEMA[i-1]) * 2 / (longnum + 1);
	}

	double shotclose = sEMA[i-1];
	double mediclose = mEMA[i-1];
	double longclose = lEMA[i-1];

	for (i = bar->head + 1; i <= bar->tail; ++i) {
		be = bar->bars[i];
		if (be == NULL) continue;	
		lEMA = smalloc(bar->num * sizeof(double));
		mEMA = smalloc(bar->num * sizeof(double));
		sEMA = smalloc(bar->num * sizeof(double));
		eb->longEMA[i] = sEMA;
		eb->mediEMA[i] = mEMA;
		eb->shotEMA[i] = sEMA;
		if (be->workingIndex == 0) return eb;
		sEMA[0] = shotclose + (be->closePrice[0] - shotclose) * 2 / (shotnum + 1);
		mEMA[0] = mediclose + (be->closePrice[0] - mediclose) * 2 / (medinum + 1);
		lEMA[0] = longclose + (be->closePrice[0] - longclose) * 2 / (longnum + 1);
		int j;
		for (j = 1; j < be->workingIndex; ++j) {
			sEMA[j] = sEMA[j-1] + (be->closePrice[j] - sEMA[j-1]) * 2 / (shotnum + 1);
			mEMA[j] = mEMA[j-1] + (be->closePrice[j] - mEMA[j-1]) * 2 / (medinum + 1);
			lEMA[j] = lEMA[j-1] + (be->closePrice[j] - lEMA[j-1]) * 2 / (longnum + 1);
		}
		shotclose = sEMA[j-1];
		mediclose = mEMA[j-1];
		longclose = lEMA[j-1]; 	
	}
	return eb;
}

void EMABAR_syn(struct EMABAR *eb, int index) {
	struct BARELEMENT *be = eb->bar->bars[eb->bar->tail];
	double *lema = eb->longEMA[eb->bar->tail];
	double *mema = eb->mediEMA[eb->bar->tail];
	double *sema = eb->shotEMA[eb->bar->tail];
	lema[index] = lema[index - 1] + (be->closePrice[index] - lema[index - 1]) * 2 / (eb->longNum + 1);
	mema[index] = mema[index - 1] + (be->closePrice[index] - mema[index - 1]) * 2 / (eb->mediNum + 1);
	sema[index] = sema[index - 1] + (be->closePrice[index] - sema[index - 1]) * 2 / (eb->shotNum + 1);
}
