#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "item.h"

FILE* getUtilityFile(const int argc, char **argv);
FILE* getBackpackFile(const int argc, char **argv);
FILE* getWeightsFile(const int argc, char **argv);
int getSolutionId(const int argc, char **argv);
double readBackpackVolume(FILE *fptr);
Vector* readItemsUtility(FILE *fptr);
Vector* readItemsWeight(FILE *fptr);
ItemsInfo* readItems(FILE* fptr_items_weight, FILE* fptr_items_utility);

#endif
