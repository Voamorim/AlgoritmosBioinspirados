#include "ag.h"
#include "solve.h"
#include "build_csv.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define FACTORIAL_TEST_RESULTS_FILE "../data/results.csv"
#define GENERATIONS_RESULTS_FILE "../data/generations.csv"

int main(int argc, char **argv){
    srand(time(NULL));

    char generations_csv[] = GENERATIONS_RESULTS_FILE;
    char results_csv[] = FACTORIAL_TEST_RESULTS_FILE;

    // Seleciona qual das execuções será executada
    int solution = 1; // Por padrão, resolve de forma simples

    for(int i = 1; i < argc; i += 2){
        if(!strcmp(argv[i], "-s")){
            solution = atoi(argv[i+1]);
            if(solution < 1 || solution > 3){
                printf("Escolha de solucao invalida (apenas 1 a 3 aceito)\n");
                return 1;
            }
        }
    }

    switch(solution){
        case 1:
            solve();
            break;
        case 2:
            generationsTest(generations_csv);
            break;
        case 3:
            factorialTest(results_csv);
            break;
    }
    return 0;
}
