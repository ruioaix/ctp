#ifndef CTP_C_API_SERVER_MONGOAPI_H
#define CTP_C_API_SERVER_MONGOAPI_H

#include "ctpcapi.h"
#include <mongoc.h>

mongoc_client_t *MongoAPI_create_client(char *url_port);
void MongoAPI_destory_client(mongoc_client_t *client);
mongoc_collection_t ** MongoAPI_glue_collections(mongoc_client_t *client, char **InstrumentIDs, int InstrumentNum, char *BrokerID, char *UserID);
void MongoAPI_unglue_collections(mongoc_collection_t **mclls, int InstrumentNum);
void insert_mongodb(mongoc_client_t *client, mongoc_collection_t *collection, CThostFtdcDepthMarketDataField *pd, double arrivedtime, double processtime);

#endif
