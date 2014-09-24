#include "bar.h"
#include "mongoapi.h"
#include "safe.h"

static void BAR_ELEMENT_index(struct BARELEMENT *bar, int hour, int minute, int *i2) {
	if (hour > 12) {
		*i2 = (hour-13)*60 + (BAR1DNUM_1M-3)/2 + 2 + minute;
	}
	else {
		*i2 = (hour-9)*60 + minute - 14;
	}
}

static void BAR_ELEMENT_ymd_hms(struct BARELEMENT *bar, int i2, int *hms) {
	int hour, minute;
	if (i2 > (BAR1DNUM_1M - 3)/2 + 1) {
		hour = (i2 - (BAR1DNUM_1M - 3)/2 - 2)/60 + 13;	
		minute = (i2 - (BAR1DNUM_1M - 3)/2 - 2)%60;
	}
	else {
		hour = (i2 + 14)/60 + 9;
		minute = (i2 + 14)%60;
	}
	*hms = hour*10000 + minute*100;
}
static struct BARELEMENT *create_BAR_ELEMENT(enum BARTYPE type) {
	struct BARELEMENT *be = smalloc(sizeof(struct BARELEMENT));
	be->isComplete = 0;
	be->YMD = 0;
	enum BAR1DNUM num;
	switch (type) {
		case BARTYPE_1M: 
			num = BAR1DNUM_1M;
			break;
		case BARTYPE_5M:
			num = BAR1DNUM_5M;
			break;
		case BARTYPE_10M:
			num = BAR1DNUM_10M;
			break;
		case BARTYPE_20M:
			num = BAR1DNUM_20M;
			break;
		case BARTYPE_1H:
			num = BAR1DNUM_1H;
			break;
		case BARTYPE_1D:
			num = BAR1DNUM_1D;
			break;
		default:
			isError("wrong");
	}
	be->btimeHMS = smalloc(num * sizeof(int));
	be->etimeHMS = smalloc(num * sizeof(int));
	be->openPrice = smalloc(num * sizeof(double));
	be->closePrice = smalloc(num * sizeof(double));
	be->uplimitPrice = smalloc(num * sizeof(double));
	be->lowlimitPrice = smalloc(num * sizeof(double));
	be->volume = smalloc(num * sizeof(int));
	int i;
	for (i = 0; i < num; ++i) {
		int hms;
		BAR_ELEMENT_ymd_hms(be, i, &hms);
		be->btimeHMS[i] = hms;
		be->etimeHMS[i] = hms+59;
		be->openPrice[i] = be->closePrice[i] = be->uplimitPrice[i] = -1;
		be->lowlimitPrice[i] = INT_MAX;
	}
	int index;
   	BAR_ELEMENT_index(be, 9, 14, &index);	
	be->etimeHMS[index] = 91400;
	BAR_ELEMENT_index(be, 11, 30, &index);	
	be->etimeHMS[index] = 113000;
	BAR_ELEMENT_index(be, 15, 15, &index);	
	be->etimeHMS[index] = 151500;
	return be;
}
static int get_BAR_ELEMENT(struct BAR *bar, int ymd) {
	if (bar->bars[bar->head] == NULL) {
		struct BARELEMENT *be = create_BAR_ELEMENT(BAR1DNUM_1M);
		bar->bars[bar->head] = be;
		return bar->head;
	}
	else {
		int hymd = bar->bars[bar->head]->YMD;
		int tymd = bar->bars[bar->tail]->YMD;
		if (hymd > ymd) {
			bar->head += ymd-hymd;
			bar->bars[bar->head] = create_BAR_ELEMENT(BAR1DNUM_1M);
			return bar->head;
		}
		else if (tymd < ymd) {
			bar->tail += ymd-tymd;
			bar->bars[bar->tail] = create_BAR_ELEMENT(BAR1DNUM_1M);
			return bar->tail;
		}
		else {
			if (bar->bars[bar->head+ymd-tymd] == NULL) {
				bar->bars[bar->head+ymd-tymd] = create_BAR_ELEMENT(BAR1DNUM_1M);
			}
			return bar->head+ymd-tymd;
		}
	}
}
static void insert_BAR_ELEMENT(struct BARELEMENT *be, int ymd, int hour, int minute, int second, int millsecond, int volume, double lastprice, int *mills, int *mills2) {
	int index;
	BAR_ELEMENT_index(be, hour, minute, &index);
	
	if (second == 0 && (mills[index] == 10000 || millsecond < mills[index])) {
		be->openPrice[index] = lastprice;
		mills[index] = millsecond;
	}
	if (second == 59 && (mills2[index] == 10000 || millsecond > mills2[index])) {
		be->closePrice[index] = lastprice;
		mills2[index] = millsecond;
	}
	be->uplimitPrice[index] = be->uplimitPrice[index] > lastprice ? be->uplimitPrice[index] : lastprice;
	be->lowlimitPrice[index] = be->lowlimitPrice[index] > lastprice ? lastprice : be->lowlimitPrice[index];
	be->volume[index] += volume;

}
struct BAR *create_1MTYPE_BAR_from_MongoDB(mongoc_collection_t *cll, int beginYMD, int endYMD) {
	int num, *ymd, *hour, *minute, *second, *millsecond, *volume;
	double *lastprice;
	MongoAPI_fetch_DMD_FOR_BAR(cll, beginYMD, endYMD, &num, &ymd, &hour, &minute, &second, &millsecond, &volume, &lastprice);

	if (num == 0) return NULL;

	int *mills = smalloc(BAR1DNUM_1M*sizeof(int));
	int *mills2 = smalloc(BAR1DNUM_1M*sizeof(int));
	int i;
	for (i = 0; i < BAR1DNUM_1M; ++i) {
		mills[i] = 10000;
		mills2[i] = 10000;
	}
	
	struct BAR *bar = smalloc(sizeof(struct BAR));
	bar->type = BARTYPE_1M;
	bar->num = BAR1DNUM_1M;
	bar->head = bar->tail = BAR_DAYSNUM_MAX/2;
	for (i = 0; i < num; ++i) {
		int index = get_BAR_ELEMENT(bar, ymd[i]);
		bar->bars[index]->YMD = ymd[i];
		insert_BAR_ELEMENT(bar->bars[index], ymd[i], hour[i], minute[i], second[i], millsecond[i], volume[i], lastprice[i], mills, mills2);
	}

	int k=0;
	for (i = bar->head+1; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) {
			k=i;
			break;
		}
	}
	if (k!=0) {
		for (i = k+1; i <= bar->tail; ++i) {
			if (bar->bars[i] != NULL) {
				bar->bars[k] = bar->bars[i];
				bar->bars[i] = NULL;
				k++;
			}
		}
		bar->tail = k;
	}
	for (i = bar->head+1; i <= bar->tail; ++i) {
		int index;
		BAR_ELEMENT_index(bar->bars[i], 9, 14, &index);	
		bar->bars[i]->closePrice[index] = bar->bars[i]->openPrice[index];
		BAR_ELEMENT_index(bar->bars[i], 11, 30, &index);	
		bar->bars[i]->closePrice[index] = bar->bars[i]->openPrice[index];
		BAR_ELEMENT_index(bar->bars[i], 15, 15, &index);	
		bar->bars[i]->closePrice[index] = bar->bars[i]->openPrice[index];
	}
	return bar;

}
