#ifndef CTP_C_API_SERVER_SAFE_H
#define CTP_C_API_SERVER_SAFE_H

#include <stdlib.h>
void *malloc_safe(size_t size, const char *funcname, const char *filename, const int lineNum);
void *calloc_safe(size_t num, size_t size, const char *funcname, const char *filename, const int lineNum);
void *realloc_safe(void *p, size_t size, const char *funcname, const char *filename, const int lineNum);
#define smalloc(n) malloc_safe(n, __func__, __FILE__, __LINE__)
#define scalloc(n, s) calloc_safe(n, s, __func__, __FILE__, __LINE__)
#define srealloc(p, n) realloc_safe(p, n, __func__, __FILE__, __LINE__)

#include <stdio.h> 
FILE *fopen_safe(const char *openfilename, const char *mode, const char *functionname, char *filename, const int lineNum);
#define sfopen(fn, mode) fopen_safe(fn, mode, __func__, __FILE__, __LINE__)

#endif//end of CTP_C_API_SERVER_SAFE_H
