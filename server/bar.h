#ifndef CTP_C_API_SERVER_BAR_H
#define CTP_C_API_SERVER_BAR_H

#define BAR_DAYSNUM_MAX 8192

//one day bar element.
struct BARELEMENT {
	//if workingIndex == num, means this BARELEMENT is complete: one day complete data.
	//else if workingIndex >=0, means the workingIndex is the index of the bar which is not finish.
	//else, workingIndex is -1, means the BARELEMENT just being inited.
	int workingIndex; 
	//default is 0, only when workingIndex >0 && workingVolume < num, the value is the sum of the [0, workingIndex-1] bar's volume.
	int workingVolume;
	//when workingIndex == -1, lastHMSM == -1.
	//when workingIndex >=0 && < num, lastHMSM is the last dmdmsg's update time&milltime.
	//when workingIndex == num, lastHMSM is 151500x00, x is the last dmdmsg's milltime.
	int lastHMSM;

	int YMD;
	int *btimeHMSM;
	int *etimeHMSM;
	double *openPrice;
	double *closePrice;
	double *uplimitPrice;
	double *lowlimitPrice;
	int *volume;
};

//one day
struct BAR {
	int barLen;
	int num;
	int head;
	int tail;
	struct BARELEMENT *bars[BAR_DAYSNUM_MAX];
};

void free_BAR(struct BAR *bar);

#include <mongoc.h>
void BARELEMENT_fill(struct BARELEMENT *be, int hour, int minute, int second, int millsecond, int volume, double lastprice);
//when: in [beginYMD, endYMD], no data in mongodb, return a empty bar.(bar->head == bar->tail && bar->bars[bar->head] == NULL)
struct BAR *create_1MTYPE_BAR_from_MongoDB(mongoc_collection_t *cll, int beginYMD, int endYMD);
struct BAR *create_BAR(int barLen, mongoc_collection_t *cll, int beginYMD, int endYMD);
void create_Multi_BAR(int num, int *barLen, mongoc_collection_t *cll, int beginYMD, int endYMD, struct BAR ***barA);

#endif
