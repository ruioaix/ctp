#ifndef CTP_C_API_SERVER_BAR_H
#define CTP_C_API_SERVER_BAR_H

struct BAR {
	int minutesNum;
	int barsNum;
	char InstrumentID[10];
	int *btime;
	int *etime;
	double *openPrice;
	double *closePrice;
	double *uplimitPrice;
	double *lowlimitPrice;
	int *volume;
};

#define BARSNUM_ONE_MINUTE 273

#endif
