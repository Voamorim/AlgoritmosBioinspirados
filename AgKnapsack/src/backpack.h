#ifndef BACKPACK
#define BACKPACK

#include "item.h"

#include <stdio.h>
#include <string.h>

typedef struct Backpack{
    int *items; // Vetor binário indexado pelo id do objeto (pertence ou não)
    int nitems;
    double volume;
    double value;
} Backpack;

Backpack* createBackpack(const int nitems, const double volume);
void assignRandomItemsToBackpack(Backpack *backpack);
double getBackpackUsedWeight(const Backpack *backpack, Item **items);
double getBackpackUtility(const Backpack *backpack, Item **items);
void copyBackpack(Backpack* dest, Backpack *src);
void freeBackpack(Backpack* backpack);

#endif
