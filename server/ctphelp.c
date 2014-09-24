#include "ctphelp.h"
#include <stdlib.h>

void CTPHELP_updatetime2HMS(const char *updatetime, int *hour, int *minute, int *second) {
	char tt[3];
	tt[2] = '\0';
	tt[0] = updatetime[0];
	tt[1] = updatetime[1];
	*hour = strtol(tt, NULL, 10);
	tt[0] = updatetime[3];
	tt[1] = updatetime[4];
	*minute = strtol(tt, NULL, 10);
	tt[0] = updatetime[6];
	tt[1] = updatetime[7];
	*second = strtol(tt, NULL, 10);
}

