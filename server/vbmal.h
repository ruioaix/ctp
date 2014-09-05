/**
 * This file may be included by any other *.c or *.h file.
 * it provide functions and preprocessor macros that could be used most commonly by  other functions.
 *
 * Author: RuiXiao <xrfind@gmail.com>
 */
#ifndef CTP_C_API_VBMAL_H
#define CTP_C_API_VBMAL_H

/****************VERBOSE LEVEL*******************************************************************/
//LEVEL  0 : all functions defined here will be extended to ((void)0).
//LEVEL  1 : basic
//LEVEL  2 : complete
//LEVEL  3 : more and more

#define VERBOSE_LEVEL 2
#define printlb(format, ...) ((void)0)
#define printlc(format, ...) ((void)0)
#define printld(format, ...) ((void)0)

#if VERBOSE_LEVEL >= 1
#undef printlb
#define printlb(format, ...) do {\
		printf("[LEVLE %d] ", VERBOSE_LEVEL);\
		printf("%s =>> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#if VERBOSE_LEVEL >= 2
#undef printlc
#define printlc(format, ...) do {\
		printf("[LEVLE %d] ", VERBOSE_LEVEL);\
		printf("%s ==> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#if VERBOSE_LEVEL >= 3
#undef printld
#define printld(format, ...) do {\
		printf("[LEVLE %d] ", VERBOSE_LEVEL);\
		printf("%s ==> ", __func__);\
		printf(format, ##__VA_ARGS__);\
		printf("\n");\
} while(0)
#endif
#endif
#endif
/********************************************************************************************************/

/*******use everywhere.**********************************************************************************/
#include <stdio.h> //for FILE, perror, fprintf, stderr
#include <stdlib.h>
#define isError(format, ...) do {\
		fflush(stdout);\
		fprintf(stderr, "[ERROR]:\n\tfile: \"%s\", line: %d.\n\t%s =>> ", \
				__FILE__, __LINE__, __func__);\
		fprintf(stderr, format, ##__VA_ARGS__);\
		fprintf(stderr, "\n");\
		exit(EXIT_FAILURE);\
} while(0)
/********************************************************************************************************/

/*********use everywhere.********************************************************************************/
void *malloc_safe(size_t size, const char *funcname, const char *filename, const int lineNum);
void *calloc_safe(size_t num, size_t size, const char *funcname, const char *filename, const int lineNum);
void *realloc_safe(void *p, size_t size, const char *funcname, const char *filename, const int lineNum);
#define smalloc(n) malloc_safe(n, __func__, __FILE__, __LINE__)
#define scalloc(n, s) calloc_safe(n, s, __func__, __FILE__, __LINE__)
#define srealloc(p, n) realloc_safe(p, n, __func__, __FILE__, __LINE__)

FILE *fopen_safe(const char *openfilename, const char *mode, const char *functionname, char *filename, const int lineNum);
#define sfopen(fn, mode) fopen_safe(fn, mode, __func__, __FILE__, __LINE__)
/********************************************************************************************************/

#endif
