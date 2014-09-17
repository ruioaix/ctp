#include "ctpcapi.h"
 
#include "io.h"
#include "vbmal.h"
#include "mongoapi.h"
#include "threadfunc.h"

#include <stdio.h>
#include <pthread.h>
#include <mongoc.h>

int main(int argc, char **argv) {
	char *file;
	if (argc == 1) {
		file = "others/xxx_2";
	}
	else if (argc == 2) {
		file = argv[1];
	}
	else {
		isError("wrong args");
	}
	char *InstrumentIDs[10]={NULL};
	int InstrumentNum = 1;
	char *logfilepath, *server, *BrokerID, *UserID, *pd, *mongodb_url_port;
	readinfo(file, &logfilepath, &server, &BrokerID, &UserID, &pd, &InstrumentIDs, &InstrumentNum, &mongodb_url_port);
	void *md = MD_create(logfilepath, server, BrokerID, UserID, pd, InstrumentIDs, InstrumentNum);

	mongoc_client_t *client = MongoAPI_create_client(mongodb_url_port);
	mongoc_collection_t **mcollections = MongoAPI_glue_collections(client, InstrumentIDs, InstrumentNum, BrokerID, UserID);

	int running = 1;
	struct MongoIM mim;
	mim.client = client;
	mim.mcollections = mcollections;
	mim.mcollectionsNum = InstrumentNum;
	mim.InstrumentIDs = InstrumentIDs;
	mim.md = md;
	mim.running = &running;

	pthread_t insert_dmdmsg;
	pthread_create(&insert_dmdmsg, NULL, ProcessDMD, &mim);
	pthread_t revise_instrment;
	pthread_create(&revise_instrment, NULL, ProcessINS, &mim);


	MD_init(md);

	sleep(20);
	running = 0;

	pthread_join(insert_dmdmsg, NULL);
	pthread_join(revise_instrment, NULL);

	free(logfilepath);
	free(server);
	free(BrokerID);
	free(UserID);
	free(pd);
	free(mongodb_url_port);
	int i;
	for (i = 0; i < InstrumentNum; ++i) {
		free(InstrumentIDs[i]);
	}
	
	MongoAPI_unglue_collections(mcollections, InstrumentNum);
	MongoAPI_destory_client(client);

	MD_free(md);
	sleep(2);
	_exit(0);
}
