#include "ctpcapi.h"
 
#include "io.h"
#include "mongoapi.h"
#include "threadfunc.h"
#include "verbose.h"
#include "bar.h"

#include <stdio.h>
#include <pthread.h>
#include <mongoc.h>

int main(int argc, char **argv) {
	//prepare md,td,mongodb.
	setbuf(stdout, (char *) 0);
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
	char *mdlogfilepath, *tdlogfilepath, *mdserver, *tdserver, *mongodb_url_port, *BrokerID, *InvestorID, *UserID, *pd, *UserProductInfo;
	readinfo(file, &mdlogfilepath, &tdlogfilepath, &mdserver, &tdserver, &mongodb_url_port, &BrokerID, &InvestorID, &UserID, &pd, &UserProductInfo, &InstrumentIDs, &InstrumentNum);

	void *md = MD_create(mdlogfilepath, mdserver, BrokerID, UserID, pd, InstrumentIDs, InstrumentNum);
	void *td = TD_create(tdlogfilepath, tdserver, BrokerID, InvestorID, UserID, pd, UserProductInfo, THOST_TERT_RESUME); 

	mongoc_client_t *client = MongoAPI_create_client(mongodb_url_port);
	mongoc_collection_t **mcollections = MongoAPI_glue_collections(client, InstrumentIDs, InstrumentNum, BrokerID, UserID);

	//connect and login to md server.
	//if ok, begin to receive dmdmsg.
	//		all the dmdmsg will be store in the queue in md.
	//		size of queue in md is 8192, it costs one hour to fill the queue.
	//if fail, most possible, it's because of the server is not open, it's ok.
	//		just wait, this function will open two thread.
	//		when the md server is open, the md server will call one thread and say:I am open.
	//		then, the other thread will connect and login to md server again.
	MD_init(md);

	//connect and login to td server
	//it's quite similar to md server's logic.
	TD_init(td);

	//prepare thread's param.
	int running = 1;
	struct ThreadIM mim;
	mim.client = client;
	mim.mcollections = mcollections;
	mim.mcollectionsNum = InstrumentNum;
	mim.InstrumentIDs = InstrumentIDs;
	mim.md = md;
	mim.td = td;
	mim.running = &running;
	mim.barLen = 5;

	//open the thread.
	//this thread is core.
	pthread_t wait_event;
	pthread_create(&wait_event, NULL, EVENT_500ms_dmdmsg, &mim);

	//wait here.
	pthread_join(wait_event, NULL);

	//free
	free(mdlogfilepath);
	free(tdlogfilepath);
	free(mdserver);
	free(tdserver);
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
	MD_free(td);
	_exit(0);
}
