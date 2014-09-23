#include "mongoapi.h"
#include "safe.h"

mongoc_client_t *MongoAPI_create_client(char *url_port) {
	mongoc_init ();
	return mongoc_client_new(url_port);
}

void MongoAPI_destory_client(mongoc_client_t *client) {
	mongoc_client_destroy(client);
}

mongoc_collection_t ** MongoAPI_glue_collections(mongoc_client_t *client, char **InstrumentIDs, int InstrumentNum, char *BrokerID, char *UserID) {
	mongoc_collection_t **mcollections = smalloc((InstrumentNum+1) * sizeof(mongoc_collection_t *));

	char mongo_collection_name[1000];
	int i;
	for (i = 0; i < InstrumentNum; ++i) {
		sprintf(mongo_collection_name, "%s_%s_%s", BrokerID, UserID, InstrumentIDs[i]);
		mcollections[i] = mongoc_client_get_collection (client, "ctp", mongo_collection_name);
	}
	sprintf(mongo_collection_name, "%s_%s_%s", BrokerID, UserID, "other");
	mcollections[i] = mongoc_client_get_collection(client, "ctp", mongo_collection_name);
	return mcollections;
}

void MongoAPI_unglue_collections(mongoc_collection_t **mcollections, int InstrumentNum) {
	int i;
	for (i = 0; i < InstrumentNum; ++i) {
		mongoc_collection_destroy (mcollections[i]);
	}
	mongoc_collection_destroy(mcollections[i]);
	free(mcollections);
}

void MongoAPI_insert_DMD(mongoc_client_t *client, mongoc_collection_t *collection, CThostFtdcDepthMarketDataField *pd) {

	bson_t *doc = BCON_NEW (
			"TradingDay", BCON_UTF8 (pd->TradingDay),
			"InstrumentID", BCON_UTF8 (pd->InstrumentID),
			"ExchangeID", BCON_UTF8 (pd->ExchangeID),
			"ExchangeInstID", BCON_UTF8 (pd->ExchangeInstID),
			"LastPrice", BCON_DOUBLE (pd->LastPrice),
			"PreSettlementPrice", BCON_DOUBLE (pd->PreSettlementPrice),
			"PreClosePrice", BCON_DOUBLE (pd->PreClosePrice),
			"PreOpenInterest", BCON_DOUBLE (pd->PreOpenInterest),
			"OpenPrice", BCON_DOUBLE (pd->OpenPrice),
			"HighestPrice", BCON_DOUBLE (pd->HighestPrice),
			"LowestPrice", BCON_DOUBLE (pd->LowestPrice),
			"Volume", BCON_INT32 (pd->Volume),
			"Turnover", BCON_DOUBLE (pd->Turnover),
			"OpenInterest", BCON_DOUBLE (pd->OpenInterest),
			"ClosePrice", BCON_DOUBLE (pd->ClosePrice),
			"SettlementPrice", BCON_DOUBLE (pd->SettlementPrice),
			"UpperLimitPrice", BCON_DOUBLE (pd->UpperLimitPrice),
			"LowerLimitPrice", BCON_DOUBLE (pd->LowerLimitPrice),
			"PreDelta", BCON_DOUBLE (pd->PreDelta),
			"CurrDelta", BCON_DOUBLE (pd->CurrDelta),
			"UpdateTime", BCON_UTF8 (pd->UpdateTime),
			"UpdateMillisec", BCON_INT32 (pd->UpdateMillisec),
			"BidPrice1", BCON_DOUBLE (pd->BidPrice1),
			"BidVolume1", BCON_INT32 (pd->BidVolume1),
			"AskPrice1", BCON_DOUBLE (pd->AskPrice1),
			"AskVolume1", BCON_INT32 (pd->AskVolume1),
			"BidPrice2", BCON_DOUBLE (pd->BidPrice2),
			"BidVolume2", BCON_INT32 (pd->BidVolume2),
			"AskPrice2", BCON_DOUBLE (pd->AskPrice2),
			"AskVolume2", BCON_INT32 (pd->AskVolume2),
			"BidPrice3", BCON_DOUBLE (pd->BidPrice3),
			"BidVolume3", BCON_INT32 (pd->BidVolume3),
			"AskPrice3", BCON_DOUBLE (pd->AskPrice3),
			"AskVolume3", BCON_INT32 (pd->AskVolume3),
			"BidPrice4", BCON_DOUBLE (pd->BidPrice4),
			"BidVolume4", BCON_INT32 (pd->BidVolume4),
			"AskPrice4", BCON_DOUBLE (pd->AskPrice4),
			"AskVolume4", BCON_INT32 (pd->AskVolume4),
			"BidPrice5", BCON_DOUBLE (pd->BidPrice5),
			"BidVolume5", BCON_INT32 (pd->BidVolume5),
			"AskPrice5", BCON_DOUBLE (pd->AskPrice5),
			"AskVolume5", BCON_INT32 (pd->AskVolume5),
			"AveragePrice", BCON_DOUBLE (pd->AveragePrice),
			"ActionDay", BCON_UTF8 (pd->ActionDay) //this item make valgrind give a cond sometime.
				);

	int mci = mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, NULL);
	while (!mci) {
		printf("mongo insert error.\n");
		sleep(1);
		mci = mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, NULL);
	}
	bson_destroy(doc);

}

static void getupdatetime(const char *updatetime, int *hour, int *minute, int *second) {
	char tt[3];
	tt[2] = '\0';
	tt[0] = updatetime[0];
	tt[1] = updatetime[1];
	*hour = strtol(tt, NULL, 10);
	tt[0] = updatetime[3];
	tt[1] = updatetime[4];
	*minute = strtol(tt, NULL, 10);
	tt[0] = updatetime[6];
	tt[1] = updatetime[7];
	*second = strtol(tt, NULL, 10);
}
static int get_index_bar(int begin_day, int tday, int hour, int minute) {
	int index = (tday - begin_day)*273;
	if (hour > 12) {
		index += (hour-13)*60 + 137 + minute;
	}
	else {
		index += (hour-9)*60 + minute - 14;
	}
	return index;
}
static int get_hourminute_bar(int begin_day, int index) {
	int day = index%273;	
	int tday = index/273 + begin_day;
	int hour, minute;
	if (day > 136) {
		hour = (day-137)/60 + 13;	
		minute = (day-137)%60;
	}
	else {
		hour = (day+14)/60 + 9;
		minute = (day+14)%60;
	}
	int year = tday%20000000;
	int time = year*10000 + hour*100 + minute;
	return time;	
}
struct BAR *MongoAPI_fetch_bar_1m(mongoc_collection_t *cll, int begin_day, int end_day) {

	mongoc_cursor_t *cursor;
	bson_error_t error;
	const bson_t *doc;
	char begind[10], endd[10];
	sprintf(begind, "%d", begin_day);
	sprintf(endd, "%d", end_day);

	bson_t *query = BCON_NEW (
			"TradingDay", "{", "$gte", BCON_UTF8(begind), "}",
			"TradingDay", "{", "$lte", BCON_UTF8(endd), "}"
			);
	cursor = mongoc_collection_find(cll, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

	struct BAR *bar = calloc(1, sizeof(struct BAR));
	int daysNum = end_day - begin_day + 1;
	bar->minutesNum = 1;
	bar->barsNum = BARSNUM_ONE_MINUTE * daysNum;
	bar->openPrice = malloc(bar->barsNum * sizeof(double));
	bar->closePrice = malloc(bar->barsNum * sizeof(double));
	bar->uplimitPrice = malloc(bar->barsNum * sizeof(double));
	bar->lowlimitPrice = malloc(bar->barsNum * sizeof(double));
	bar->volume = malloc(bar->barsNum * sizeof(int));
	bar->btime = malloc(bar->barsNum * sizeof(int));
	bar->etime = malloc(bar->barsNum * sizeof(int));

	int *mills = malloc(bar->barsNum * sizeof(int));

	int i;
	for (i = 0; i < bar->barsNum; ++i) {
		mills[i] = 10000;
		bar->btime[i] = bar->etime[i] = get_hourminute_bar(begin_day, i);
	}

	bson_iter_t iter;
	int kk = 0;
	while (mongoc_cursor_next(cursor, &doc)) {
		int tday, hour, minute, second, millsecond;
		double lastprice;
		int volume;
		tday = hour = minute = second = volume = -1;
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
					getupdatetime(bson_iter_utf8(&iter, NULL), &hour, &minute, &second);
					//printf("%s\t%d\t%d\t%d\n", key, hour, minute, second);
				}
				else if (strcmp(key, "TradingDay") == 0) {
					tday = strtol(bson_iter_utf8(&iter,NULL), NULL, 10);
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
		int index = get_index_bar(begin_day, tday, hour, minute);
		if (second == 0 && (mills[index] == 10000 || millsecond < mills[index])) {
			bar->openPrice[index] = lastprice;
			mills[index] = millsecond;
		}
		if (second == 59 && (mills[index] == 10000 || millsecond > mills[index])) {
			bar->closePrice[index] = lastprice;
			mills[index] = millsecond;
		}
		bar->uplimitPrice[index] = bar->uplimitPrice[index] > lastprice ? bar->uplimitPrice[index] : lastprice;
		bar->lowlimitPrice[index] = bar->lowlimitPrice[index] > lastprice ? lastprice : bar->lowlimitPrice[index];
		bar->volume[index] += volume;
	}
	printf("/********************************************************************************************************/\n");
	printf("%d\n", kk);

	if (mongoc_cursor_error (cursor, &error)) {
		fprintf (stderr, "An error occurred: %s\n", error.message);
	}

	mongoc_cursor_destroy (cursor);
	bson_destroy (query);

	return bar;
}
