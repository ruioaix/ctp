#ifndef CTP_C_API_SERVER_MONGOAPI_H
#define CTP_C_API_SERVER_MONGOAPI_H

#include "ctpcapi.h"
#include "bar.h"
#include <mongoc.h>

mongoc_client_t *MongoAPI_create_client(char *url_port);
void MongoAPI_destory_client(mongoc_client_t *client);
mongoc_collection_t ** MongoAPI_glue_collections(mongoc_client_t *client, char **InstrumentIDs, int InstrumentNum, char *BrokerID, char *UserID);
void MongoAPI_unglue_collections(mongoc_collection_t **mclls, int InstrumentNum);
void MongoAPI_insert_DMD(mongoc_client_t *client, mongoc_collection_t *collection, CThostFtdcDepthMarketDataField *pd);

struct BAR *MongoAPI_fetch_bar_1m(mongoc_collection_t *cll, int begin_day, int end_day);

#endif
