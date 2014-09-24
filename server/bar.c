#include "bar.h"

int BAR_index(struct BAR *bar, int ymd, int hour, int minute) {
	if (bar->type == BAR_ONE_M) {
		int index = (ymd - bar->beginYMD) * (BARSNUM_ONE_MINUTE_HALFDAY*2 + 3);
		if (hour > 12) {
			index += (hour-13)*60 + BARSNUM_ONE_MINUTE_HALFDAY + 2 + minute;
		}
		else {
			index += (hour-9)*60 + minute - 14;
		}
		return index;
	}
	return -1;
}

void BAR_ymd_hms(struct BAR *bar, int index, int *ymd, int *hms) {
	if (bar->type == BAR_ONE_M) {
		int hmsIndex = index%(BARSNUM_ONE_MINUTE_HALFDAY*2 + 3);
		*ymd = index/(BARSNUM_ONE_MINUTE_HALFDAY*2 + 3) + bar->beginYMD;
		int hour, minute;
		if (hmsIndex > BARSNUM_ONE_MINUTE_HALFDAY + 1) {
			hour = (hmsIndex - BARSNUM_ONE_MINUTE_HALFDAY - 2)/60 + 13;	
			minute = (hmsIndex - BARSNUM_ONE_MINUTE_HALFDAY - 2)%60;
		}
		else {
			hour = (hmsIndex+14)/60 + 9;
			minute = (hmsIndex+14)%60;
		}
		*hms = hour*10000 + minute*100;
	}
}
