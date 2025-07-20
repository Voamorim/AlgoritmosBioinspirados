#ifndef BUILD_CSV
#define BUILD_CSV

#include <stdio.h>

FILE* createCSV(char *file_name);
void writeLineCSV(FILE *fptr, char **values, const int n);
void closeCSV(FILE *fptr);


#endif
