#include "safe.h"

void *malloc_safe(size_t size, const char *funcname, const char *filename, const int lineNum) {
	void *tmp = malloc(size);
	if (tmp == NULL) {
		fprintf(stderr, "[ERROR]:\n\t%s => malloc failed: \"%s\" file, %d line.\n", funcname, filename, lineNum);
		exit(EXIT_FAILURE);
	}
	return tmp;
}
void *calloc_safe(size_t num, size_t size, const char *funcname, const char *filename, const int lineNum) {
	void *tmp = calloc(num, size);
	if (tmp == NULL) {
		fprintf(stderr, "[ERROR]:\n\t%s => calloc failed: \"%s\" file, %d line.\n", funcname, filename, lineNum);
		exit(EXIT_FAILURE);
	}
	return tmp;
}
void *realloc_safe(void *p, size_t size, const char *funcname, const char *filename, const int lineNum) {
	void *tmp = realloc(p, size);
	if (tmp == NULL) {
		fprintf(stderr, "[ERROR]:\n\t%s => realloc failed: \"%s\" file, %d line.\n", funcname, filename, lineNum);
		exit(EXIT_FAILURE);
	}
	return tmp;
}

FILE *fopen_safe(const char *openfilename, const char *mode, const char *funcname, char *filename, const int lineNum) {
	FILE *fp = fopen(openfilename, mode);
	if (fp == NULL) {
		fprintf(stderr, "[ERROR]:\n\t%s => fopen failed: can not open \"%s\" file, location: \"%s\", %d line.\n", funcname, openfilename, filename, lineNum);
		exit(EXIT_FAILURE);
	}
	return fp;
}
