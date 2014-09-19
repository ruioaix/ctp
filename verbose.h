/**
 * This file may be included by any other *.c or *.h file.
 * it provide functions and preprocessor macros that could be used most commonly by  other functions.
 *
 * Author: RuiXiao <xrfind@gmail.com>
 */
#ifndef CTP_C_API_SERVER_VERBOSE_H
#define CTP_C_API_SERVER_VERBOSE_H

#include <stdio.h>
#include <stdlib.h>
/****************VERBOSE LEVEL*******************************************************************/
//LEVEL  0 : all functions defined here will be extended to ((void)0).
//LEVEL  1 : basic
//LEVEL  2 : complete
//LEVEL  3 : more and more

#define printmlb(format, ...) ((void)0)
#define printmlc(format, ...) ((void)0)
#define printmld(format, ...) ((void)0)
#define printtlb(format, ...) ((void)0)
#define printtlc(format, ...) ((void)0)
#define printtld(format, ...) ((void)0)

#define MD_VERBOSE_LEVEL 0
#define TD_VERBOSE_LEVEL 2

#if MD_VERBOSE_LEVEL >= 1
#undef printmlb
#define printmlb(format, ...) do {\
		printf("[MdUser LEVLE 1] ");\
		printf("%s =>> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#if MD_VERBOSE_LEVEL >= 2
#undef printmlc
#define printmlc(format, ...) do {\
		printf("[MdUser LEVLE 2] ");\
		printf("%s ==> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#if MD_VERBOSE_LEVEL >= 3
#undef printmld
#define printmld(format, ...) do {\
		printf("[MdUser LEVLE 3] ");\
		printf("%s ==> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#endif
#endif
#endif

#if TD_VERBOSE_LEVEL >= 1
#undef printtlb
#define printtlb(format, ...) do {\
		printf("[Trader LEVLE 1] ");\
		printf("%s =>> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#if TD_VERBOSE_LEVEL >= 2
#undef printtlc
#define printtlc(format, ...) do {\
		printf("[Trader LEVLE 2] ");\
		printf("%s ==> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#if TD_VERBOSE_LEVEL >= 3
#undef printtld
#define printtld(format, ...) do {\
		printf("[Trader LEVLE 3] ");\
		printf("%s ==> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#endif
#endif
#endif

#define isError(format, ...) do {\
		fflush(stdout);\
		fprintf(stderr, "[ERROR]:\n\tfile: \"%s\", line: %d.\n\t%s =>> ", \
				__FILE__, __LINE__, __func__);\
		fprintf(stderr, format, ##__VA_ARGS__);\
		fprintf(stderr, "\n");\
		exit(EXIT_FAILURE);\
} while(0)

#endif//end of CTP_C_API_SERVER_VERBOSE_H
