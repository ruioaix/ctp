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

struct BAR *create_BAR_F_MongoDB(mongoc_collection_t *cll, enum BAR_TYPE type, int beginYMD, int endYMD) {
	/*
	struct BAR_METADATA *bmd = MongoAPI_fetch_DMD_FOR_BAR(cll, beginYMD, endYMD);

	//bar 
	struct BAR *bar = smalloc(sizeof(struct BAR));
	bar->type = BAR_ONE_M;
	bar->barsNum = (BARSNUM_ONE_MINUTE_HALFDAY*2 + 3) * (endYMD - beginYMD+ 1);
	bar->beginYMD = beginYMD;

	bar->YMD = scalloc(bar->barsNum, sizeof(int));
	bar->btimeHMS = scalloc(bar->barsNum, sizeof(int));
	bar->etimeHMS = scalloc(bar->barsNum, sizeof(int));
	bar->openPrice = smalloc(bar->barsNum * sizeof(double));
	bar->closePrice = smalloc(bar->barsNum * sizeof(double));
	bar->uplimitPrice = smalloc(bar->barsNum * sizeof(double));
	bar->lowlimitPrice = smalloc(bar->barsNum * sizeof(double));
	bar->volume = scalloc(bar->barsNum, sizeof(int));

	int *mills = malloc(bar->barsNum * sizeof(int));
	int *mills2 = malloc(bar->barsNum * sizeof(int));

	int i;
	for (i = 0; i < bar->barsNum; ++i) {
		mills[i] = 10000;
		mills2[i] = 10000;
		int ymd, hms;
		BAR_ymd_hms(bar, i, &ymd, &hms);
		bar->YMD[i] = ymd;
		bar->btimeHMS[i] = hms;
		bar->etimeHMS[i] = hms+59;
		bar->openPrice[i] = bar->closePrice[i] = bar->uplimitPrice[i] = -1;
		bar->lowlimitPrice[i] = INT_MAX;
	}

	bson_iter_t iter;
	int kk = 0;
	while (mongoc_cursor_next(cursor, &doc)) {
		int ymd, hour, minute, second, millsecond;
		double lastprice;
		int volume;
		ymd = hour = minute = second = volume = -1;
		lastprice = -1;
		kk++;
		if (bson_iter_init(&iter, doc)) {
			//double openPrice, closePrice, uplimitPrice, lowlimitPrice;
			while(bson_iter_next(&iter)) {
				const char *key = bson_iter_key(&iter);
				if (strcmp(key, "Volume") == 0) {
					volume = (int)bson_iter_int64(&iter);
					//printf("%s\t%d\n", key, volume);
				}
				else if (strcmp(key, "UpdateTime") == 0) {
					CTPHELP_updatetime2HMS(bson_iter_utf8(&iter, NULL), &hour, &minute, &second);
					//printf("%s\t%d\t%d\t%d\n", key, hour, minute, second);
				}
				else if (strcmp(key, "TradingDay") == 0) {
					ymd = strtol(bson_iter_utf8(&iter,NULL), NULL, 10);
					//printf("%s\t%d\n", key, tday);
				}
				else if (strcmp(key, "LastPrice") == 0) {
					lastprice = bson_iter_double(&iter);
					//printf("%s\t%f\n", key, lastprice);
				}
				else if (strcmp(key, "UpdateMillisec") == 0) {
					millsecond = (int)bson_iter_int64(&iter);
				}
			}
		}
		int index = BAR_index(bar, ymd, hour, minute);
		if (second == 0 && (mills[index] == 10000 || millsecond < mills[index])) {
			bar->openPrice[index] = lastprice;
			mills[index] = millsecond;
		}
		if (second == 59 && (mills2[index] == 10000 || millsecond > mills2[index])) {
			bar->closePrice[index] = lastprice;
			mills2[index] = millsecond;
		}
		bar->uplimitPrice[index] = bar->uplimitPrice[index] > lastprice ? bar->uplimitPrice[index] : lastprice;
		bar->lowlimitPrice[index] = bar->lowlimitPrice[index] > lastprice ? lastprice : bar->lowlimitPrice[index];
		bar->volume[index] += volume;
	}

	for (i = beginYMD; i <= endYMD; ++i) {
		int index = BAR_index(bar, i, 9, 14);	
		bar->etimeHMS[index] = 91400;
		bar->closePrice[index] = bar->openPrice[index];
		index = BAR_index(bar, i, 11, 30);	
		bar->etimeHMS[index] = 113000;
		bar->closePrice[index] = bar->openPrice[index];
		index = BAR_index(bar, i, 15, 15);	
		bar->etimeHMS[index] = 151500;
		bar->closePrice[index] = bar->openPrice[index];
	}


	free(mills);
	free(mills2);
	printf("%d\n", kk);

	if (mongoc_cursor_error (cursor, &error)) {
		fprintf (stderr, "An error occurred: %s\n", error.message);
	}

	mongoc_cursor_destroy (cursor);
	bson_destroy (query);


	return bar;
	*/
	return NULL;
}
