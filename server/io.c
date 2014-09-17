#include "io.h"
#include "vbmal.h"
#include <string.h>

void readinfo(char *filename, char **logfilepath, char **server, char **BrokerID, char **UserID, char **pd, char *(*InstrumentIDs)[], int *InstrumentNum, char **mongodb_url_port) {
	FILE *fp = sfopen(filename, "r");
	char line[1000];
	int i = 0;
	int j = 0;
	while(fgets(line, 1000, fp)) {
		switch(i++) {
			case 0:
				*logfilepath = smalloc(strlen(line));
				memcpy(*logfilepath, line, strlen(line));
				(*logfilepath)[strlen(line)-1] = '\0';
				break;
			case 1:
				*server = smalloc(strlen(line));
				memcpy(*server, line, strlen(line));
				(*server)[strlen(line)-1] = '\0';
				break;
			case 2:
				*BrokerID= smalloc(strlen(line));
				memcpy(*BrokerID, line, strlen(line));
				(*BrokerID)[strlen(line)-1] = '\0';
				break;
			case 3:
				*UserID= smalloc(strlen(line));
				memcpy(*UserID, line, strlen(line));
				(*UserID)[strlen(line)-1] = '\0';
				break;
			case 4:
				*pd = smalloc(strlen(line));
				memcpy(*pd, line, strlen(line));
				(*pd)[strlen(line)-1] = '\0';
				break;
			case 5:
				*mongodb_url_port = smalloc(strlen(line));
				memcpy(*mongodb_url_port, line, strlen(line));
				(*mongodb_url_port)[strlen(line)-1] = '\0';
				break;
			default:
				(*InstrumentIDs)[j] = smalloc(strlen(line));
				memcpy((*InstrumentIDs)[j], line, strlen(line));
				(*InstrumentIDs)[j][strlen(line)-1] = '\0';
				j++;
				break;
		}
	}
	*InstrumentNum = j;
	fclose(fp);
}