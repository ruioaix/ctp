#ifndef CTP_C_API_SERVER_THREADFUNC_H
#define CTP_C_API_SERVER_THREADFUNC_H


#include <mongoc.h>

struct MongoIM {
	void *md;
	mongoc_client_t *client;
	mongoc_collection_t **mcollections;
	int mcollectionsNum;
	char **InstrumentIDs;	
	int *running;
};

void *ProcessDMD(void *mim_p);
void *ProcessINS(void *mim_p);

#endif
