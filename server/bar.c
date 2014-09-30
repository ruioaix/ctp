#include "bar.h"
#include "mongoapi.h"
#include "safe.h"

#define BAR_METADATA_1DNUM_MAX 32500
#define BARNUM_1MIN1DAY 273
#define BARNUM_1MIN1DAY_HALF 135
#define BARLEN_HALFDAY_MIN 60
#define BARLEN_1DAY_MIN 135

#define ONEE3 1000
#define ONEE5 100000
#define ONEE7 10000000

//free bar
static void free_BARELEMENT(struct BARELEMENT *be) {
	free(be->btimeHMSM);
	free(be->etimeHMSM);
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

//this BARELEMENT_num function is the core logic in BAR's divide.
//need to taste.
//suppose: barLen > 0
static int BARELEMENT_num(int barLen) {
	if (barLen > BARLEN_1DAY_MIN) return 1;
	if (barLen > BARLEN_HALFDAY_MIN) return 2;
	if (barLen == 1) return BARNUM_1MIN1DAY;
	int num = BARNUM_1MIN1DAY_HALF/barLen;
	if (BARNUM_1MIN1DAY_HALF%barLen != 0) {
		++num;
	}
	return num*2;
}
//suppose: barLen > 0 && index is always valid.
//return the index's btimeHMS.
//this BARELEMENT_hms function is also the core logic in BAR's divide.
//and more clear than BARELEMENT_num
static void BARELEMENT_hms(int barLen, int index, int *bHMS, int *eHMS) {
	if (barLen > BARLEN_1DAY_MIN) {
		*bHMS = 91400;
		*eHMS = 151500;
		return;
	}
	if (barLen > BARLEN_HALFDAY_MIN) {
		if (index == 0) {
			*bHMS = 91400;
			*eHMS = 113000;
			return;
		}
		*bHMS = 130000;
		*eHMS = 151500;
		return;
	}
	if (barLen == 1) {
		int hour, minute;
		if (index > 136) {
			hour = (index - 137)/60 + 13;	
			minute = (index - 137)%60;
		}
		else {
			hour = (index + 14)/60 + 9;
			minute = (index + 14)%60;
		}
		*bHMS = hour*10000 + minute*100;
		if (index == 0 || index == 136 || index == 272) {
			*eHMS = *bHMS;
		}
		else {
			*eHMS = *bHMS + 59;
		}
		return;
	}
	int num = BARELEMENT_num(barLen);
	int hour, minute;
	if (index > num/2 - 1) {
		hour = (index - num/2)*barLen/60 + 13;	
		minute = (index - num/2)*barLen%60;
	}
	else {
		hour = (index*barLen+ 15)/60 + 9;
		minute = (index*barLen+ 15)%60;
	}
	if (index == 0) {
		*bHMS = 91400;
		*eHMS = 91500 + (barLen-1)*100 + 59;
		return;
	}
	if (index == num/2 - 1) {
		*bHMS = hour*10000 + minute*100;
		*eHMS = 113000;
		return;
	}
	if (index == num - 1) {
		*bHMS = hour*10000 + minute*100;
		*eHMS = 151500;
		return;
	}
	*bHMS = hour*10000 + minute*100;
	*eHMS = *bHMS + (barLen-1)*100 + 59;
	return;
}
//this BARELEMENT_index function is also the core logic in BAR's divide.
//suppose barLen > 0 && hour&minute is valid
static int BARELEMENT_index(int barLen, int hour, int minute) {
	if (barLen > BARLEN_1DAY_MIN) return 0;
	if (barLen > BARLEN_HALFDAY_MIN) {
		if (hour > 12) return 1;
		return 0;
	}
	if (barLen == 1) {
		if (hour > 12) {
			return (hour-13)*60 + 137 + minute;
		}
		else {
			return (hour-9)*60 + minute - 14;
		}
	}
	if (hour == 9 && minute == 14) return 0;
	if (hour == 11 && minute == 30) return BARELEMENT_num(barLen)/2-1;
	if (hour == 15 && minute == 15) return BARELEMENT_num(barLen)-1;
	if (hour > 12) {
		int afterbegin = BARELEMENT_num(barLen)/2;
		return ((hour-13)*60 + minute)/barLen + afterbegin;
	}
	else {
		return ((hour-9)*60 + minute - 15)/barLen;
	}
}
//suppose barLen > 0
static struct BARELEMENT *create_BARELEMENT(int barLen, int ymd) {
	struct BARELEMENT *be = smalloc(sizeof(struct BARELEMENT));
	be->workingIndex = -1;
	be->workingVolume = 0;
	be->lastHMSM = -1;
	be->YMD = ymd;
	int num = BARELEMENT_num(barLen);
	be->btimeHMSM = smalloc(num * sizeof(int));
	be->etimeHMSM = smalloc(num * sizeof(int));
	be->openPrice = smalloc(num * sizeof(double));
	be->closePrice = smalloc(num * sizeof(double));
	be->uplimitPrice = smalloc(num * sizeof(double));
	be->lowlimitPrice = smalloc(num * sizeof(double));
	be->volume = scalloc(num, sizeof(int));
	int i;
	for (i = 0; i < num; ++i) {
		int bhms, ehms;
	   	BARELEMENT_hms(barLen, i, &bhms, &ehms);
		be->btimeHMSM[i] = bhms*1000 + 999;
		be->etimeHMSM[i] = ehms*1000;
		be->openPrice[i] = be->closePrice[i] = be->uplimitPrice[i] = -1;
		be->lowlimitPrice[i] = INT_MAX;
	}
	return be;
}
struct BARELEMENT *BAR_find_BE(struct BAR *bar, int ymd) {
	if (bar->bars[bar->head] == NULL) {
		bar->bars[bar->head] = create_BARELEMENT(bar->barLen, ymd);
		return bar->bars[bar->head];
	}
	int hymd = bar->bars[bar->head]->YMD;
	int tymd = bar->bars[bar->tail]->YMD;
	if (hymd > ymd) {
		bar->head -= hymd - ymd;
		bar->bars[bar->head] = create_BARELEMENT(bar->barLen, ymd);
		return bar->bars[bar->head];
	}
	if (tymd < ymd) {
		bar->tail += ymd-tymd;
		bar->bars[bar->tail] = create_BARELEMENT(bar->barLen, ymd);
		return bar->bars[bar->tail];
	}
	if (bar->bars[bar->head+ymd-hymd] == NULL) {
		bar->bars[bar->head+ymd-hymd] = create_BARELEMENT(bar->barLen, ymd);
	}
	return bar->bars[bar->head+ymd-hymd];
}
//if one dmdmsg has been filled twice, nothing happen. it's ok.
void BARELEMENT_fill(struct BARELEMENT *be, int barLen, int hour, int minute, int second, int millsecond, int volume, double lastprice) {
	//get index
	int index = BARELEMENT_index(barLen, hour, minute);
	//get workingIndex index, default is -1.
	be->workingIndex = be->workingIndex > index ? be->workingIndex : index;
	//get dmdmsg's HMSM.
	int HMSM = hour*ONEE7 + minute*ONEE5 + second*ONEE3 + millsecond;
	//get be's lastHMSM
	be->lastHMSM = be->lastHMSM > HMSM ? be->lastHMSM : HMSM;
	//by default, btimeHMSM is xxxxxx999.
	if (HMSM <= be->btimeHMSM[index]) {
		be->openPrice[index] = lastprice;
		be->btimeHMSM[index] = HMSM;
	}
	//by default, etimeHMSM is xxxxxx000.
	if (HMSM >= be->etimeHMSM[index]) {
		be->closePrice[index] = lastprice;
		be->etimeHMSM[index] = HMSM;
	}
	//uplimitPrice default is -1.
	be->uplimitPrice[index] = be->uplimitPrice[index] > lastprice ? be->uplimitPrice[index] : lastprice;
	//lowlimitPrice default is INT_MAX.
	be->lowlimitPrice[index] = be->lowlimitPrice[index] > lastprice ? lastprice : be->lowlimitPrice[index];
	//volume default is 0.
	be->volume[index] = be->volume[index] > volume ? be->volume[index] : volume;
}
//from and to may point to the same BE
//suppose: barLen is between [2, 60]
//suppose: from is a 1m be
//suppose: to is created with param this barLen.
static void BARELEMENT_evolution(struct BARELEMENT *from, struct BARELEMENT *to, int barLen) {
	int num = BARELEMENT_num(barLen);

	int j,k;
	//morning first
	//91400 is included, so totally use barLen+1.
	to->btimeHMSM[0] = from->btimeHMSM[0];
	to->etimeHMSM[0] = from->etimeHMSM[barLen];
	to->openPrice[0] = from->openPrice[0];
	to->closePrice[0] = from->closePrice[barLen];
	to->uplimitPrice[0] = from->uplimitPrice[0];
	to->lowlimitPrice[0] = from->lowlimitPrice[0];
	to->volume[0] = from->volume[0];
	for (j = 1; j < barLen+1; ++j) {
		to->uplimitPrice[0] = to->uplimitPrice[0] > from->uplimitPrice[j] ? to->uplimitPrice[0] : from->uplimitPrice[j];
		to->lowlimitPrice[0] = to->lowlimitPrice[0] < from->lowlimitPrice[j] ? to->lowlimitPrice[0] : from->lowlimitPrice[j];
		to->volume[0] = to->volume[0] > from->volume[j] ? to->volume[0] : from->volume[j];
	}
	//morning middle
	//totally use barLen.
	for (j = 1; j < num/2 - 1; ++j) {
		to->btimeHMSM[j] = from->btimeHMSM[barLen*j+1];
		to->etimeHMSM[j] = from->etimeHMSM[barLen*(1+j)];
		to->openPrice[j] = from->openPrice[barLen*j+1];
		to->closePrice[j] = from->closePrice[barLen*(1+j)];
		to->uplimitPrice[j] = from->uplimitPrice[barLen*j+1];
		to->lowlimitPrice[j] = from->lowlimitPrice[barLen*j+1];
		to->volume[j] = from->volume[barLen*j+1];
		for (k =2; k <= barLen; ++k) {
			to->uplimitPrice[j] = to->uplimitPrice[j] > from->uplimitPrice[j*barLen+k] ? to->uplimitPrice[j] : from->uplimitPrice[j*barLen + k];
			to->lowlimitPrice[j] = to->lowlimitPrice[j] < from->lowlimitPrice[j*barLen + k] ? to->lowlimitPrice[j] : from->lowlimitPrice[j*barLen + k];
			to->volume[j] = to->volume[j] > from->volume[j*barLen + k] ? to->volume[j] : from->volume[j*barLen + k];
		}
	}
	//morning last
	//113000 is included.
	//totally use 2~(barLen+1).
	j = num/2 - 1;
	to->btimeHMSM[j] = from->btimeHMSM[barLen*j+1];
	to->etimeHMSM[j] = from->etimeHMSM[136];
	to->openPrice[j] =  from->openPrice[barLen*j+1];
	to->closePrice[j] = from->closePrice[136];
	to->uplimitPrice[j] =  from->uplimitPrice[barLen*j+1];
	to->lowlimitPrice[j] = from->lowlimitPrice[barLen*j+1];
	to->volume[j] = from->volume[barLen*j+1];
	for (k =2; j*barLen + k < 137; ++k) {
		to->uplimitPrice[j] = to->uplimitPrice[j] > from->uplimitPrice[j*barLen+k] ? to->uplimitPrice[j] : from->uplimitPrice[j*barLen + k];
		to->lowlimitPrice[j] = to->lowlimitPrice[j] < from->lowlimitPrice[j*barLen + k] ? to->lowlimitPrice[j] : from->lowlimitPrice[j*barLen + k];
		to->volume[j] = to->volume[j] > from->volume[j*barLen + k] ? to->volume[j] : from->volume[j*barLen + k];
	}
	//afternoon first and middle
	//totally use barLen
	for (j = num/2; j < num-1; ++j) {
		int startpoint = 137+barLen*(j-num/2);
		to->btimeHMSM[j] = from->btimeHMSM[startpoint];
		to->etimeHMSM[j] = from->etimeHMSM[startpoint + barLen - 1];
		to->openPrice[j] = from->openPrice[startpoint];
		to->closePrice[j] = from->closePrice[startpoint + barLen - 1];
		to->uplimitPrice[j] = from->uplimitPrice[startpoint];
		to->lowlimitPrice[j] = from->lowlimitPrice[startpoint];
		to->volume[j] = from->volume[startpoint];
		for (k =1; k < barLen; ++k) {
			to->uplimitPrice[j] = to->uplimitPrice[j] > from->uplimitPrice[startpoint + k] ? to->uplimitPrice[j] : from->uplimitPrice[startpoint + k];
			to->lowlimitPrice[j] = to->lowlimitPrice[j] < from->lowlimitPrice[startpoint + k] ? to->lowlimitPrice[j] : from->lowlimitPrice[startpoint + k];
			to->volume[j] = to->volume[j] > from->volume[startpoint + k] ? to->volume[j] : from->volume[startpoint + k];
		}
	}
	//afternoon last
	//totally use 2~(barLen+1).
	j = num - 1;
	int startpoint = 137 + barLen*(num/2 - 1);
	to->btimeHMSM[j] = from->btimeHMSM[startpoint];
	to->etimeHMSM[j] = from->etimeHMSM[272];
	to->openPrice[j] = from->openPrice[startpoint];
	to->closePrice[j] = from->closePrice[272];
	to->uplimitPrice[j] = from->uplimitPrice[startpoint];
	to->lowlimitPrice[j] = from->lowlimitPrice[startpoint];
	to->volume[j] = from->volume[startpoint];
	for (k =1; startpoint + k < 273; ++k) {
		to->uplimitPrice[j] = to->uplimitPrice[j] > from->uplimitPrice[startpoint + k] ? to->uplimitPrice[j] : from->uplimitPrice[startpoint + k];
		to->lowlimitPrice[j] = to->lowlimitPrice[j] < from->lowlimitPrice[startpoint + k] ? to->lowlimitPrice[j] : from->lowlimitPrice[startpoint + k];
		to->volume[j] = to->volume[j] > from->volume[startpoint + k] ? to->volume[j] : from->volume[startpoint + k];
	}
}
//from and to may point to the same BE
//suppose: from is a 1m be
//suppose: to is halfdaybe or 1mbe
static void BARELEMENT_evolution_halfday(struct BARELEMENT *from, struct BARELEMENT *to) {
	//morning
	to->btimeHMSM[0] = from->btimeHMSM[0];
	to->etimeHMSM[0] = from->etimeHMSM[136];
	to->openPrice[0] = from->openPrice[0];
	to->closePrice[0] = from->closePrice[136];
	to->uplimitPrice[0] = from->uplimitPrice[0];
	to->lowlimitPrice[0] = from->lowlimitPrice[0];
	to->volume[0] = from->volume[0];
	int j;
	for (j = 1; j < 137; ++j) {
		to->uplimitPrice[0] = to->uplimitPrice[0] > from->uplimitPrice[j] ? to->uplimitPrice[0] : from->uplimitPrice[j];
		to->lowlimitPrice[0] = to->lowlimitPrice[0] < from->lowlimitPrice[j] ? to->lowlimitPrice[0] : from->lowlimitPrice[j];
		to->volume[0] = to->volume[0] > from->volume[j] ? to->volume[0] : from->volume[j];
	}
	//afternoon
	to->btimeHMSM[1] = from->btimeHMSM[137];
	to->etimeHMSM[1] = from->etimeHMSM[272];
	to->openPrice[1] = from->openPrice[137];
	to->closePrice[1] = from->closePrice[272];
	to->uplimitPrice[1] = from->uplimitPrice[137];
	to->lowlimitPrice[1] = from->lowlimitPrice[272];
	to->volume[1] = from->volume[137];
	for (j = 138; j < 273; ++j) {
		to->uplimitPrice[1] = to->uplimitPrice[1] > from->uplimitPrice[j] ? to->uplimitPrice[1] : from->uplimitPrice[j];
		to->lowlimitPrice[1] = to->lowlimitPrice[1] < from->lowlimitPrice[j] ? to->lowlimitPrice[1] : from->lowlimitPrice[j];
		to->volume[1] = to->volume[1] > from->volume[j] ? to->volume[1] : from->volume[j];
	}
}
//from and to may point to the same BE
//suppose: from is a 1m be
//suppose: to is 1daybe or 1mbe
static void BARELEMENT_evolution_1day(struct BARELEMENT *from, struct BARELEMENT *to) {
	to->btimeHMSM[0] = from->btimeHMSM[0];
	to->etimeHMSM[0] = from->etimeHMSM[272];
	to->openPrice[0] = from->openPrice[0];
	to->closePrice[0] = from->closePrice[272];
	to->volume[0] = from->volume[0];
	int j;
	for (j = 1; j < 273; ++j) {
		to->uplimitPrice[0] = to->uplimitPrice[0] > from->uplimitPrice[j] ? to->uplimitPrice[0] : from->uplimitPrice[j];
		to->lowlimitPrice[0] = to->lowlimitPrice[0] < from->lowlimitPrice[j] ? to->lowlimitPrice[0] : from->lowlimitPrice[j];
		to->volume[0] = to->volume[0] > from->volume[j] ? to->volume[0] : from->volume[j];
	}
}
static struct BAR *init_BAR(int barLen) {
	struct BAR *bar = smalloc(sizeof(struct BAR));
	bar->barLen = barLen;
	bar->num = BARELEMENT_num(barLen);
	bar->head = bar->tail = BAR_DAYSNUM_MAX/2;
	int i;
	for (i = 0; i < BAR_DAYSNUM_MAX; ++i) {
		bar->bars[i] = NULL;	
	}
	return bar;
}
struct BAR *create_BAR_from_MongoDB(int barLen, mongoc_collection_t *cll, int beginYMD, int endYMD) {
	struct BAR *bar = init_BAR(barLen);
	
	int num, memnum, *hour, *minute, *second, *millsecond, *volume;
	double *lastprice;
	memnum = BAR_METADATA_1DNUM_MAX;
	hour = scalloc(memnum, sizeof(int)); 
	minute = scalloc(memnum, sizeof(int)); 
	second = scalloc(memnum, sizeof(int)); 
	millsecond = scalloc(memnum, sizeof(int));
	volume = scalloc(memnum, sizeof(int));
	lastprice = smalloc(memnum * sizeof(double));

	int i,j;
	for (i = beginYMD; i <= endYMD; ++i) {
		//always try to fetch oneday's dmdmsg data.
		//if the i is today, the dmdmsg is not complete oneday dmdmsg data.
		//if the i is any date before today, the dmdmsg is 100% complete oneday dmdmsg data.
		MongoAPI_fetch_DMD_FOR_BAR(cll, i, &num, &memnum, &hour, &minute, &second, &millsecond, &volume, &lastprice);
		if (num == 0) continue;
		//get here, means there is valid dmdmsg. so workingIndex will not be -1 anymore, lastHMSM too.
		struct BARELEMENT *be = BAR_find_BE(bar, i);
		for (j = 0; j < num; ++j) {
			BARELEMENT_fill(be, barLen, hour[j], minute[j], second[j], millsecond[j], volume[j], lastprice[j]);
		}
		if (be->lastHMSM >= 151500000) {
			be->workingIndex = BARNUM_1MIN1DAY;
			be->workingVolume = 0;
		}
	}
	free(hour);free(minute);free(second);
	free(millsecond);free(volume);free(lastprice);
	return bar;
}

//suppose: bar argument's barLen == 1 
static struct BAR *create_BAR_function_halfday(struct BAR *bar) {
	int i;
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		BARELEMENT_evolution_halfday(be, be);
	}
	bar->barLen = BARNUM_1MIN1DAY_HALF;
	bar->num = 2;
	return bar;
}
//suppose: bar argument's barLen == 1 
static struct BAR *create_BAR_function_1day(struct BAR *bar) {
	int i;
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		BARELEMENT_evolution_1day(be, be);
	}
	bar->barLen = BARNUM_1MIN1DAY;
	bar->num = 1;
	return bar;
}
//suppose: bar argument's barLen == 1
static struct BAR *create_BAR_function_notlarge60m(struct BAR *bar, int barLen) {
	int i;
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		BARELEMENT_evolution(be, be, barLen);
	}
	bar->barLen = barLen;
	bar->num = BARELEMENT_num(barLen);
	return bar;
}
struct BAR *convert_BAR_from_1MBAR(int barLen, struct BAR *bar) {
	if (barLen > 135) {
		return create_BAR_function_1day(bar);
	}
	if (barLen > 60) {
		return create_BAR_function_halfday(bar);
	}
	if (barLen > 1) {
		return create_BAR_function_notlarge60m(bar, barLen);
	}
	if (barLen == 1) return bar;
	return NULL;
}

/****until here, all functions is not limit to "barLen == 1"*********************************************/
//suppose: bar argument's barLen == 1
static struct BAR *create_Multi_BAR_function_notlarge60m(struct BAR *bar, int barLen) {
	struct BAR *newbar = smalloc(sizeof(struct BAR));
	newbar->barLen = barLen;
	newbar->num = BARELEMENT_num(barLen);
	newbar->head = bar->head;
	newbar->tail = bar->tail;
	int i;
	for (i = 0; i < BAR_DAYSNUM_MAX; ++i) {
		newbar->bars[i] = NULL;	
	}
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		struct BARELEMENT *nbe = newbar->bars[i] = create_BARELEMENT(barLen, be->YMD);
		BARELEMENT_evolution(be, nbe, barLen);
	}
	return newbar;
}
//suppose: bar argument's barLen == 1
static struct BAR *create_Multi_BAR_function_halfday(struct BAR *bar) {
	struct BAR *newbar = smalloc(sizeof(struct BAR));
	newbar->head = bar->head;
	newbar->tail = bar->tail;
	int i;
	for (i = 0; i < BAR_DAYSNUM_MAX; ++i) {
		newbar->bars[i] = NULL;	
	}
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		struct BARELEMENT *newbe = newbar->bars[i] = create_BARELEMENT(60, be->YMD);
		BARELEMENT_evolution_halfday(be, newbe);
	}
	newbar->barLen = BARNUM_1MIN1DAY_HALF;
	newbar->num = 2;
	return newbar;
}
//suppose: bar argument's barLen == 1
static struct BAR *create_Multi_BAR_function_1day(struct BAR *bar) {
	struct BAR *newbar = smalloc(sizeof(struct BAR));
	newbar->head = bar->head;
	newbar->tail = bar->tail;
	int i;
	for (i = 0; i < BAR_DAYSNUM_MAX; ++i) {
		newbar->bars[i] = NULL;	
	}
	for (i = bar->head; i <= bar->tail; ++i) {
		if (bar->bars[i] == NULL) continue;
		struct BARELEMENT *be = bar->bars[i];
		struct BARELEMENT *newbe = newbar->bars[i] = create_BARELEMENT(60, be->YMD);
		BARELEMENT_evolution_1day(be, newbe);
	}
	newbar->barLen = BARNUM_1MIN1DAY;
	newbar->num = 1;
	return newbar;
}
void create_Multi_BAR(int num, int *barLenA, mongoc_collection_t *cll, int beginYMD, int endYMD, struct BAR ***barA) {
	*barA = smalloc(63*sizeof(struct BAR **));
	int i;
	for (i = 0; i < 63; ++i) {
		(*barA)[i] = NULL;	
	}
	struct BAR *bar = create_BAR_from_MongoDB(1, cll, beginYMD, endYMD);
	for (i = 0; i < num; ++i) {
		int barLen = barLenA[i];	
		if (barLen > 135) {
			if ((*barA)[62] == NULL) {
				(*barA)[62] = create_Multi_BAR_function_1day(bar);
			}
		}
		else if (barLen > 60) {
			if ((*barA)[61] == NULL) {
				(*barA)[61] = create_Multi_BAR_function_halfday(bar);
			}
		}
		else if (barLen > 1) {
			if ((*barA)[barLen] == NULL) {
				(*barA)[barLen] = create_Multi_BAR_function_notlarge60m(bar, barLen);
			}
		}
		else if (barLen == 1) {
			if ((*barA)[1] == NULL) {
				(*barA)[1] = bar;
			}
		}
	}
	if ((*barA)[1] == NULL) {
		free_BAR(bar);
	}
}
