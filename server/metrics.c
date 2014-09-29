#include "metrics.h"
#include "safe.h"

struct EMABAR *create_EMABAR(struct BAR *bar, int longnum, int medinum, int shotnum) {
	if (bar == NULL || bar->bars[bar->head] == NULL) return NULL;
	struct EMABAR *eb = smalloc(sizeof(struct EMABAR));
	eb->bar = bar;
	double *longEMA = smalloc(bar->num * sizeof(double));
	double *mediEMA = smalloc(bar->num * sizeof(double));
	double *shotEMA = smalloc(bar->num * sizeof(double));
	struct BARELEMENT *be = bar->bars[bar->head];
	shotEMA[0] = mediEMA[0] = longEMA[0] = be->closePrice[0];
	int i;
	for (i = 1; i < bar->num; ++i) {
		shotEMA[i] = shotEMA[i-1] + (be->closePrice[i] - shotEMA[i-1]) * 2 / (shotnum + 1);
		mediEMA[i] = mediEMA[i-1] + (be->closePrice[i] - mediEMA[i-1]) * 2 / (medinum + 1);
		longEMA[i] = longEMA[i-1] + (be->closePrice[i] - longEMA[i-1]) * 2 / (longnum + 1);
	}
	//double shotclose = shotEMA[i-1];
	for (i = bar->head + 1; i <= bar->tail; ++i) {
		
	}
	return NULL;
}
