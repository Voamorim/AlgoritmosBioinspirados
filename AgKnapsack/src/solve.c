#include "ag.h"
#include "item.h"
#include "io.h"

#include <stdio.h>

void solve(FILE* items_weights_fptr, FILE* items_utility_fptr,
           FILE* backpack_volume_fptr){
    const int npop = 200;
    const int ngen = 200;
    const int nelite = 2;
    const double pcrossover= 1.0f;
    const double pmutation = 0.1f;

    ItemsInfo* items_info = readItems(items_weights_fptr, items_utility_fptr);
    double backpack_volume = readBackpackVolume(backpack_volume_fptr);

    AG *ag = buildAG(npop, ngen, pcrossover, pmutation, nelite, items_info->nitems,
                  backpack_volume, items_info->items);

    if(ag == NULL){
        printf("Erro ao criar o algoritmo genetico.\n");
        exit(1);
    }

    createInitialPopulation(ag);

    for(int generation = 0; generation < ag->ngen; ++generation){
        evaluateFitness(ag); // Avalia os fitness dos indivíduos
        int *vparents = roulette(ag);
        crossover(ag, vparents);
        mutation(ag);
        elitism(ag);

        copyPopulation(ag);

        //printPopulation(ag);

        free(vparents); // Libera o vetor de pais da geração atual da memória
    }

    printResults(ag); // Imprime os resultados finais do algoritmo genético
    freeAG(ag); // Libera a estrutura do algoritmo genético da memória

    freeItemsInfo(items_info);
}

void solveFactorialTest(const int npop, const int ngen, const double pcrossover,
                        const double pmutation, const int nelite,
                        const int nitems, const double backpack_volume,
                        Item **items, FILE* results){

    AG *ag = buildAG(npop, ngen, pcrossover, pmutation, nelite, nitems,
                      backpack_volume, items);

    if(ag == NULL){
        printf("Erro ao criar o algoritmo genetico.\n");
        exit(1);
    }

    createInitialPopulation(ag);

    for(int generation = 0; generation < ag->ngen; ++generation){
        evaluateFitness(ag);

        int* vparents = roulette(ag);
        crossover(ag, vparents);
        free(vparents);

        mutation(ag);
        elitism(ag);

        copyPopulation(ag);
    }

    printFinalResultsCSV(ag, results);
    freeAG(ag);
}

void factorialTest(char *results_file_name, FILE* items_weights_fptr,
                   FILE* items_utility_fptr, FILE* backpack_volume_fptr){

    const int npop[] = {25, 50, 75, 100, 200};
    const int ngen[] = {25, 50, 75, 100, 200};
    const int nelite[] = {0, 1, 2, 6, 10};
    const double pmutation[] = {0.01, 0.05, 0.10};
    const double pcrossover[] = {1.0, 0.80, 0.60};

    ItemsInfo* items_info = readItems(items_weights_fptr, items_utility_fptr);
    double backpack_volume = readBackpackVolume(backpack_volume_fptr);

    FILE *results_fptr = createCSV(results_file_name);
    printFinalResultsIdfCSV(results_fptr);

    for(int ipop = 0; ipop < 5; ++ipop){
        for(int igen = 0; igen < 5; ++igen){
            for(int ielite = 0; ielite < 5; ++ielite){
                for(int ipmutation = 0; ipmutation < 3; ++ipmutation){
                    for(int ipcrossover = 0; ipcrossover < 3; ++ipcrossover){
                        solveFactorialTest(npop[ipop], ngen[igen],
                                           pcrossover[ipcrossover],
                                           pmutation[ipmutation],
                                           nelite[ielite], items_info->nitems,
                                           backpack_volume, items_info->items,
                                           results_fptr);
                    }
                }
            }
        }
    }

    closeCSV(results_fptr);

    freeItemsInfo(items_info);
}

void solveGenerationsTest(const int npop, const int ngen, const double pcrossover,
                          const double pmutation, const int nelite,
                          const int nitems, const double backpack_volume,
                          Item **items, FILE *generation_results){

    AG *ag = buildAG(npop, ngen, pcrossover, pmutation, nelite, nitems,
                      backpack_volume, items);

    if(ag == NULL){
        printf("Erro ao criar o algoritmo genetico.\n");
        exit(1);
    }

    createInitialPopulation(ag);

    printGenIdfCSV(ag, generation_results);

    for(int generation = 0; generation < ag->ngen; ++generation){
        evaluateFitness(ag);

        int *vparents = roulette(ag);
        crossover(ag, vparents);
        free(vparents);

        mutation(ag);
        elitism(ag);

        copyPopulation(ag);

        printGenResultsCSV(ag, generation_results, generation + 1);
    }

    freeAG(ag);
}

void generationsTest(char *generations_file_name, FILE* items_weights_fptr,
                   FILE* items_utility_fptr, FILE* backpack_volume_fptr){

    const int npop = 200;
    const int ngen = 200;
    const double pcrossover = 1.0f;
    const double pmutation = 0.1f;
    const int nelite = 2;

    ItemsInfo* items_info = readItems(items_weights_fptr, items_utility_fptr);
    double backpack_volume = readBackpackVolume(backpack_volume_fptr);

    FILE *generations_fptr = createCSV(generations_file_name);

    solveGenerationsTest(npop, ngen, pcrossover, pmutation, nelite,
                         items_info->nitems, backpack_volume, items_info->items,
                         generations_fptr);

    closeCSV(generations_fptr);

    freeItemsInfo(items_info);
}
