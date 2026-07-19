#include "build_csv.h"

#include <stdlib.h>
#include <stdio.h>

FILE* createCSV(char *file_name){
    FILE *fptr = fopen(file_name, "w");

    return fptr;
}

void writeLineCSV(FILE *fptr, char **values, const int n){
    if(!fptr) {
        printf("Erro ao tentar escrever linha em arquivo CSV."
                "O arquivo nao existe.\n");
        return;
    }

    if(n > 0){
        fprintf(fptr, "%s", values[0]);
    }

    for(int i = 1; i < n; ++i){
        fprintf(fptr, ",");
        fprintf(fptr, "%s", values[i]);

    }

    fprintf(fptr, "\n");
}

void closeCSV(FILE *fptr){
    if(fptr) fclose(fptr);
    else{
        printf("Erro ao fechar arquivo CSV. O arquivo não existe.\n");
    }
}
