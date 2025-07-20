#ifndef SOLVE_H
#define SOLVE_H

#include "item.h"
#include "ag.h"

void solve(FILE* items_weights_fptr, FILE* items_utility_fptr,
           FILE* backpack_volume_fptr);

void solveFactorialTest(const int npop, const int ngen, const double pcrossover,
                        const double pmutation, const int nelite,
                        const int nitems, const double backpack_volume,
                        Item **items, FILE* results);

void factorialTest(char *results_file_name, FILE* items_weights_fptr,
                   FILE* items_utility_fptr, FILE* backpack_volume_fptr);

void solveGenerationsTest(const int npop, const int ngen, const double pcrossover,
                          const double pmutation, const int nelite,
                          const int nitems, const double backpack_volume,
                          Item **items, FILE *generation_results);

void generationsTest(char *generations_file_name, FILE* items_weights_fptr,
                   FILE* items_utility_fptr, FILE* backpack_volume_fptr);

#endif
