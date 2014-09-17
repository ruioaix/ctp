#ifndef CTP_C_API_SERVER_MONGO_H
#define CTP_C_API_SERVER_MONGO_H

//#include <bson.h>
#include <mongoc.h>
#include "ctpcapi.h"

mongoc_client_t *MangoAPI_init(char *url_port);
mongoc_collection_t ** MangoAPI_glue_collections(mongoc_client_t *client, char **InstrumentIDs, int InstrumentNum, char *BrokerID, char *UserID);
void insert_mongodb(mongoc_client_t *client, mongoc_collection_t *collection, CThostFtdcDepthMarketDataField *pd, double arrivedtime, double processtime);

#endif
