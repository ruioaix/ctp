#include "header.h"
#include <stdio.h>
#include <unistd.h>

void OnFrontConnected_i(void * pApi) {
	printf("xx\n");
}

int main(int argc, char **argv) {
	void *pMd = MD_CreateMdApi();
	MD_RegOnFrontConnected(pMd, OnFrontConnected_i);
	MD_Connect(pMd, "/tmp/", "tcp://222.66.97.241:41213", "9016766", "9016766", "1111111");
	MD_Subscribe(pMd, "IF1409", "IF1410");
	sleep(3);
	return 0;
}
