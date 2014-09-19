#include "io.h"
#include "safe.h"
#include <string.h>


void readinfo(char *filename, char **mdlogfilepath, char **tdlogfilepath, char **server, char **BrokerID, char **UserID, char **UserProductInfo, char **pd, char *(*InstrumentIDs)[], int *InstrumentNum, char **mongodb_url_port) {
	FILE *fp = sfopen(filename, "r");
	char line[1000];
	int i = 0;
	int j = 0;
	while(fgets(line, 1000, fp)) {
		switch(i++) {
			case 0:
				*mdlogfilepath = smalloc(strlen(line));
				memcpy(*mdlogfilepath, line, strlen(line));
				(*mdlogfilepath)[strlen(line)-1] = '\0';
				break;
			case 1:
				*tdlogfilepath = smalloc(strlen(line));
				memcpy(*tdlogfilepath, line, strlen(line));
				(*tdlogfilepath)[strlen(line)-1] = '\0';
				break;
			case 2:
				*server = smalloc(strlen(line));
				memcpy(*server, line, strlen(line));
				(*server)[strlen(line)-1] = '\0';
				break;
			case 3:
				*BrokerID= smalloc(strlen(line));
				memcpy(*BrokerID, line, strlen(line));
				(*BrokerID)[strlen(line)-1] = '\0';
				break;
			case 4:
				*UserID= smalloc(strlen(line));
				memcpy(*UserID, line, strlen(line));
				(*UserID)[strlen(line)-1] = '\0';
				break;
			case 5:
				*pd = smalloc(strlen(line));
				memcpy(*pd, line, strlen(line));
				(*pd)[strlen(line)-1] = '\0';
				break;
			case 6:
				*mongodb_url_port = smalloc(strlen(line));
				memcpy(*mongodb_url_port, line, strlen(line));
				(*mongodb_url_port)[strlen(line)-1] = '\0';
				break;
			case 7:
				*UserProductInfo = smalloc(strlen(line));
				memcpy(*UserProductInfo, line, strlen(line));
				(*UserProductInfo)[strlen(line)-1] = '\0';
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
