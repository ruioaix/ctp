#ifndef CTP_C_API_SERVER_THREADFUNC_H
#define CTP_C_API_SERVER_THREADFUNC_H


#include <mongoc.h>

struct ThreadIM {
	void *md;
	void *td;
	mongoc_client_t *client;
	mongoc_collection_t **mcollections;
	int mcollectionsNum;
	char **InstrumentIDs;	
	int *running;
	int barLen;
};

void *DMDMSG_insertIntoMongoDB(void *ThreadIM);
void *INSTRMENT_revise(void *ThreadIM);
void *EVENT_500ms_dmdmsg(void *ThreadIM);

#endif
