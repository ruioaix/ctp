#include "mongoapi.h"
#include "safe.h"
#include "ctphelp.h"
#include "bar.h"

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

/*
struct BAR *MongoAPI_fetch_1mbar(mongoc_collection_t *cll, int beginYMD, int endYMD) {

	//mongo find data.
	mongoc_cursor_t *cursor;
	bson_error_t error;
	const bson_t *doc;
	char begind[10], endd[10];
	sprintf(begind, "%d", beginYMD);
	sprintf(endd, "%d", endYMD);
	bson_t *query = BCON_NEW (
			"TradingDay", "{", "$gte", BCON_UTF8(begind), "}",
			"TradingDay", "{", "$lte", BCON_UTF8(endd), "}"
			);
	cursor = mongoc_collection_find(cll, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

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
}
*/

void MongoAPI_fetch_DMD_FOR_BAR(mongoc_collection_t *cll, int beginYMD, int endYMD, int *num, int **ymd, int **hour, int **minute, int ** second, int **millsecond, int **volume, double **lastprice) {
	if (beginYMD > endYMD) {
		return;
	}

	//bar_metadata
	*num = BAR_METADATA_MAX_NUM_ONEDAY * (endYMD - beginYMD+ 1);
	*ymd = scalloc(*num, sizeof(int));
	*hour = scalloc(*num, sizeof(int)); 
	*minute = scalloc(*num, sizeof(int)); 
	*second = scalloc(*num, sizeof(int)); 
	*millsecond = scalloc(*num, sizeof(int));
	*volume = scalloc(*num, sizeof(int));
	*lastprice = scalloc(*num, sizeof(double));

	//mongo find data.
	mongoc_cursor_t *cursor;
	bson_error_t error;
	const bson_t *doc;
	char begind[10], endd[10];
	sprintf(begind, "%d", beginYMD);
	sprintf(endd, "%d", endYMD);
	bson_t *query = BCON_NEW (
			"TradingDay", "{", "$gte", BCON_UTF8(begind), "}",
			"TradingDay", "{", "$lte", BCON_UTF8(endd), "}"
			);
	cursor = mongoc_collection_find(cll, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
	bson_iter_t iter;
	int i = 0;
	while (mongoc_cursor_next(cursor, &doc)) {
		if (bson_iter_init(&iter, doc)) {
			//double openPrice, closePrice, uplimitPrice, lowlimitPrice;
			while(bson_iter_next(&iter)) {
				const char *key = bson_iter_key(&iter);
				if (strcmp(key, "Volume") == 0) {
					(*volume)[i] = (int)bson_iter_int64(&iter);
					//printf("%s\t%d\n", key, volume);
				}
				else if (strcmp(key, "UpdateTime") == 0) {
					CTPHELP_updatetime2HMS(bson_iter_utf8(&iter, NULL), *hour+i, *minute+i, *second+i);
					//printf("%s\t%d\t%d\t%d\n", key, hour, minute, second);
				}
				else if (strcmp(key, "TradingDay") == 0) {
					(*ymd)[i] = strtol(bson_iter_utf8(&iter,NULL), NULL, 10);
					//printf("%s\t%d\n", key, tday);
				}
				else if (strcmp(key, "LastPrice") == 0) {
					(*lastprice)[i] = bson_iter_double(&iter);
					//printf("%s\t%f\n", key, lastprice);
				}
				else if (strcmp(key, "UpdateMillisec") == 0) {
					(*millsecond)[i] = (int)bson_iter_int64(&iter);
				}
			}
			i++;
			if (i==*num) {
				isError("something wrong, i should not be *num");
			}
		}
	}
	if (mongoc_cursor_error (cursor, &error)) {
		fprintf (stderr, "An error occurred: %s\n", error.message);
	}
	mongoc_cursor_destroy (cursor);
	bson_destroy (query);
}
