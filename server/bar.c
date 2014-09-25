#include "bar.h"
#include "mongoapi.h"
#include "safe.h"

static void free_BARELEMENT(struct BARELEMENT *be) {
	free(be->btimeHMS);
	free(be->etimeHMS);
	free(be->openPrice);
	free(be->closePrice);
	free(be->uplimitPrice);
	free(be->lowlimitPrice);
	free(be->volume);
	free(be);
}
void free_BAR(struct BAR *bar) {
	int i;
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;	
		free_BARELEMENT(bar->bars[i]);
	}
	free(bar);
}


static int BARELEMENT_num(int barLen) {
	if (barLen == 1) return BARNUM_1MIN1DAY;
	int num = BARNUM_1MIN1DAY_HALF/barLen;
	if (BARNUM_1MIN1DAY_HALF%barLen != 0) {
		++num;
	}
	return num*2;
}
static int BARELEMENT_hms(int barLen, int index) {
	if (barLen == 1) {
		int hour, minute;
		if (index > (BARNUM_1MIN1DAY - 3)/2 + 1) {
			hour = (index - (BARNUM_1MIN1DAY - 3)/2 - 2)/60 + 13;	
			minute = (index - (BARNUM_1MIN1DAY - 3)/2 - 2)%60;
		}
		else {
			hour = (index + 14)/60 + 9;
			minute = (index + 14)%60;
		}
		return hour*10000 + minute*100;
	}
	else {
		int num = BARELEMENT_num(barLen);
		int hour, minute;
		if (index >= num/2) {
			hour = (index - num/2)*barLen/60 + 13;	
			minute = (index - num/2)*barLen%60;
		}
		else {
			hour = (index*barLen+ 15)/60 + 9;
			minute = (index*barLen+ 15)%60;
		}
		return hour*10000 + minute*100;
	}
}
static int BARELEMENT_index(int barLen, int hour, int minute) {
	if (barLen == 1) {
		if (hour > 12) {
			return (hour-13)*60 + BARNUM_1MIN1DAY_HALF + 2 + minute;
		}
		else {
			return (hour-9)*60 + minute - 14;
		}
	}
	else {
		if (hour > 12) {
			int afterbegin = BARNUM_1MIN1DAY_HALF/barLen;
			if (BARNUM_1MIN1DAY_HALF%barLen != 0) {
				++afterbegin;
			}
			return ((hour-13)*60 + minute)/barLen + afterbegin;
		}
		else {
			return ((hour-9)*60 + minute - 15)/barLen;
		}
	}
}
static struct BARELEMENT *create_BARELEMENT(int barLen, int ymd) {
	struct BARELEMENT *be = smalloc(sizeof(struct BARELEMENT));
	be->isComplete = 0;
	be->YMD = ymd;
	int num = BARELEMENT_num(barLen);
	be->btimeHMS = smalloc(num * sizeof(int));
	be->etimeHMS = smalloc(num * sizeof(int));
	be->openPrice = smalloc(num * sizeof(double));
	be->closePrice = smalloc(num * sizeof(double));
	be->uplimitPrice = smalloc(num * sizeof(double));
	be->lowlimitPrice = smalloc(num * sizeof(double));
	be->volume = scalloc(num, sizeof(int));
	int i;
	for (i = 0; i < num; ++i) {
		int hms = BARELEMENT_hms(barLen, i);
		be->btimeHMS[i] = hms;
		be->etimeHMS[i] = hms+(barLen-1)*100+59;
		be->openPrice[i] = be->closePrice[i] = be->uplimitPrice[i] = -1;
		be->lowlimitPrice[i] = INT_MAX;
	}
	if (barLen == 1) {
		be->etimeHMS[0] = 91400;
	}
	int index = BARELEMENT_index(barLen, 11, 30);	
	be->etimeHMS[index] = 113000;
	index = BARELEMENT_index(barLen, 15, 15);	
	be->etimeHMS[index] = 151500;
	return be;
}
static struct BARELEMENT *BAR_find_BE(struct BAR *bar, int ymd) {
	if (bar->bars[bar->head] == NULL) {
		bar->bars[bar->head] = create_BARELEMENT(bar->barLen, ymd);
		return bar->bars[bar->head];
	}
	else {
		int hymd = bar->bars[bar->head]->YMD;
		int tymd = bar->bars[bar->tail]->YMD;
		if (hymd > ymd) {
			bar->head -= hymd - ymd;
			bar->bars[bar->head] = create_BARELEMENT(bar->barLen, ymd);
			return bar->bars[bar->head];
		}
		else if (tymd < ymd) {
			bar->tail += ymd-tymd;
			bar->bars[bar->tail] = create_BARELEMENT(bar->barLen, ymd);
			return bar->bars[bar->tail];
		}
		else {
			if (bar->bars[bar->head+ymd-hymd] == NULL) {
				bar->bars[bar->head+ymd-hymd] = create_BARELEMENT(bar->barLen, ymd);
			}
			return bar->bars[bar->head+ymd-hymd];
		}
	}
}
static void BARELEMENT_fill(struct BARELEMENT *be, int barLen, int hour, int minute, int second, int millsecond, int volume, double lastprice, int *mills_open, int *mills_close) {
	int index = BARELEMENT_index(barLen, hour, minute);
	if (second == 0 && (mills_open[index] == 10000 || millsecond < mills_open[index])) {
		be->openPrice[index] = lastprice;
		mills_open[index] = millsecond;
	}
	if (second == 59 && (mills_close[index] == 10000 || millsecond > mills_close[index])) {
		be->closePrice[index] = lastprice;
		mills_close[index] = millsecond;
	}
	be->uplimitPrice[index] = be->uplimitPrice[index] > lastprice ? be->uplimitPrice[index] : lastprice;
	be->lowlimitPrice[index] = be->lowlimitPrice[index] > lastprice ? lastprice : be->lowlimitPrice[index];
	be->volume[index] = be->volume[index] > volume ? be->volume[index] : volume;
}
struct BAR *create_1MTYPE_BAR_from_MongoDB(mongoc_collection_t *cll, int beginYMD, int endYMD) {
	int num, memnum, *hour, *minute, *second, *millsecond, *volume;
	double *lastprice;
	memnum = BAR_METADATA_1DNUM_MAX;
	hour = scalloc(memnum, sizeof(int)); 
	minute = scalloc(memnum, sizeof(int)); 
	second = scalloc(memnum, sizeof(int)); 
	millsecond = scalloc(memnum, sizeof(int));
	volume = scalloc(memnum, sizeof(int));
	lastprice = smalloc(memnum * sizeof(double));

	struct BAR *bar = smalloc(sizeof(struct BAR));
	bar->barLen = 1;
	bar->num = BARNUM_1MIN1DAY;
	bar->head = bar->tail = BAR_DAYSNUM_MAX/2;
	int *mills_open = smalloc(BARNUM_1MIN1DAY*sizeof(int));
	int *mills_close = smalloc(BARNUM_1MIN1DAY*sizeof(int));
	int i;
	for (i = 0; i < BAR_DAYSNUM_MAX; ++i) {
		bar->bars[i] = NULL;	
	}
	int j;
	for (i = beginYMD; i <= endYMD; ++i) {
		MongoAPI_fetch_DMD_FOR_BAR(cll, i, &num, &memnum, &hour, &minute, &second, &millsecond, &volume, &lastprice);
		if (num == 0) continue;
		struct BARELEMENT *be = BAR_find_BE(bar, i);
		for (j = 0; j < BARNUM_1MIN1DAY; ++j) {
			mills_open[j] = 10000;
			mills_close[j] = 10000;
		}
		for (j = 0; j < num; ++j) {
			BARELEMENT_fill(be, 1, hour[j], minute[j], second[j], millsecond[j], volume[j], lastprice[j], mills_open, mills_close);
		}
		for (j = BARNUM_1MIN1DAY - 1; j > 0; --j) {
			be->volume[j] -= be->volume[j-1];
		}
		be->closePrice[0] = be->openPrice[0];
		be->closePrice[136] = be->openPrice[136];
		be->closePrice[272] = be->openPrice[272];
	}

	free(hour);free(minute);free(second);
	free(millsecond);free(volume);free(lastprice);
	free(mills_open);free(mills_close);

	return bar;
}

struct BAR *create_BAR(int barLen, mongoc_collection_t *cll, int beginYMD, int endYMD) {
	struct BAR *bar = create_1MTYPE_BAR_from_MongoDB(cll, beginYMD, endYMD);
	if (barLen == 1) return bar;
	if (barLen > 135) {
		//TODO return 1d bar.
	}
	else if (barLen > 60) {
		//TODO return 0.5d bar.
	}
	//TODO the others are normal min bar.
	//now bar->num at least 6
	bar->num = BARELEMENT_num(barLen);

	int i,j,k;
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		//morning first
		be->etimeHMS[0] = be->etimeHMS[barLen];
		be->closePrice[0] = be->closePrice[barLen];
		for (j = 1; j < barLen+1; ++j) {
			be->uplimitPrice[0] = be->uplimitPrice[0] > be->uplimitPrice[j] ? be->uplimitPrice[0] : be->uplimitPrice[j];
			be->lowlimitPrice[0] = be->lowlimitPrice[0] < be->lowlimitPrice[j] ? be->lowlimitPrice[0] : be->lowlimitPrice[j];
			be->volume[0] += be->volume[j];
		}
		//morning middle
		for (j = 1; j < bar->num/2 - 1; ++j) {
			be->btimeHMS[j] = be->btimeHMS[barLen*j+1];
			be->etimeHMS[j] = be->etimeHMS[barLen*(1+j)];
			be->openPrice[j] = be->openPrice[barLen*j+1];
			be->closePrice[j] = be->closePrice[barLen*(1+j)];
			be->uplimitPrice[j] = be->uplimitPrice[barLen*j+1];
			be->lowlimitPrice[j] = be->lowlimitPrice[barLen*j+1];
			be->volume[j] = be->volume[barLen*j+1];
			for (k =2; k <= barLen; ++k) {
				be->uplimitPrice[j] = be->uplimitPrice[j] > be->uplimitPrice[j*barLen+k] ? be->uplimitPrice[j] : be->uplimitPrice[j*barLen + k];
				be->lowlimitPrice[j] = be->lowlimitPrice[j] < be->lowlimitPrice[j*barLen + k] ? be->lowlimitPrice[j] : be->lowlimitPrice[j*barLen + k];
				be->volume[j] += be->volume[j*barLen + k];
			}
		}
		//morning last
		j = bar->num/2 - 1;
		be->btimeHMS[j] = be->btimeHMS[barLen*j+1];
		be->etimeHMS[j] = be->etimeHMS[136];
		be->openPrice[j] = be->openPrice[barLen*j+1];
		be->closePrice[j] = be->closePrice[136];
		be->uplimitPrice[j] = be->uplimitPrice[barLen*j+1];
		be->lowlimitPrice[j] = be->lowlimitPrice[barLen*j+1];
		be->volume[j] = be->volume[barLen*j+1];
		for (k =2; k <= barLen && j*barLen + k < 137; ++k) {
			be->uplimitPrice[j] = be->uplimitPrice[j] > be->uplimitPrice[j*barLen+k] ? be->uplimitPrice[j] : be->uplimitPrice[j*barLen + k];
			be->lowlimitPrice[j] = be->lowlimitPrice[j] < be->lowlimitPrice[j*barLen + k] ? be->lowlimitPrice[j] : be->lowlimitPrice[j*barLen + k];
			be->volume[j] += be->volume[j*barLen + k];
		}
		//afternoon first and middle
		for (j = bar->num/2; j < bar->num-1; ++j) {
			int startpoint = 137+barLen*(j-bar->num/2);
			be->btimeHMS[j] = be->btimeHMS[startpoint];
			be->etimeHMS[j] = be->etimeHMS[startpoint+barLen - 1];
			be->openPrice[j] = be->openPrice[startpoint];
			be->closePrice[j] = be->closePrice[startpoint + barLen - 1];
			be->uplimitPrice[j] = be->uplimitPrice[startpoint];
			be->lowlimitPrice[j] = be->lowlimitPrice[startpoint];
			be->volume[j] = be->volume[startpoint];
			for (k =1; k < barLen; ++k) {
				be->uplimitPrice[j] = be->uplimitPrice[j] > be->uplimitPrice[startpoint + k] ? be->uplimitPrice[j] : be->uplimitPrice[startpoint + k];
				be->lowlimitPrice[j] = be->lowlimitPrice[j] < be->lowlimitPrice[startpoint + k] ? be->lowlimitPrice[j] : be->lowlimitPrice[startpoint + k];
				be->volume[j] += be->volume[startpoint + k];
			}
		}
		//afternoon last
		j = bar->num - 1;
		int startpoint = 137 + barLen*(bar->num/2 - 1);
		be->btimeHMS[j] = be->btimeHMS[startpoint];
		be->etimeHMS[j] = be->etimeHMS[272];
		be->openPrice[j] = be->openPrice[startpoint];
		be->closePrice[j] = be->closePrice[272];
		be->uplimitPrice[j] = be->uplimitPrice[startpoint];
		be->lowlimitPrice[j] = be->lowlimitPrice[startpoint];
		be->volume[j] = be->volume[startpoint];
		for (k =1; k < barLen && startpoint + k < 273; ++k) {
			be->uplimitPrice[j] = be->uplimitPrice[j] > be->uplimitPrice[startpoint + k] ? be->uplimitPrice[j] : be->uplimitPrice[startpoint + k];
			be->lowlimitPrice[j] = be->lowlimitPrice[j] < be->lowlimitPrice[startpoint + k] ? be->lowlimitPrice[j] : be->lowlimitPrice[startpoint + k];
			be->volume[j] += be->volume[startpoint + k];
		}
	}
	return bar;
}
