#ifndef CTP_C_API_SERVER_IO_H
#define CTP_C_API_SERVER_IO_H

void readinfo(char *filename, char **mdlogfilepath, char **tdlogfilepath, char **server, char **BrokerID, char **UserID, char **pd, char *(*InstrumentIDs)[], int *InstrumentNum, char **mongodb_url_port);

#endif
