#ifndef CTP_C_API_SERVER_IO_H
#define CTP_C_API_SERVER_IO_H

void readinfo(char *filename, char **mdlogfilepath, char **tdlogfilepath, char **mdserver, char **tdserver, char **mongodb_url_port, char **BrokerID, char **InvestorID, char **UserID, char **pd, char **UserProductInfo, char *(*InstrumentIDs)[], int *InstrumentNum);

#endif
