#include "backpack.h"
#include "item.h"

#include <stdlib.h>

// Cria e retorna uma mochila com 0 itens
Backpack* createBackpack(const int nitems, const double volume){
    Backpack* backpack = (Backpack*) malloc (sizeof(Backpack));

    backpack->items = (int*) calloc (nitems + 1, sizeof(int));

    if(!backpack->items){
        printf("Erro ao alocar memoria para os itens da mochila.\n");
        free(backpack);
        return NULL;
    }

    backpack->nitems = nitems;
    backpack->volume = volume;

    return backpack;
}

// Define os itens da mochila de forma aleatória
void assignRandomItemsToBackpack(Backpack *backpack){
    for(int i = 0; i < backpack->nitems; ++i){
        int r = rand() % 2;

        backpack->items[i] = r;
    }
}

// Calcula e retorna o peso consumido pelos itens da mochila
double getBackpackUsedWeight(const Backpack *backpack, Item **items){
    double sum = 0.0f;
    for(int i = 0; i < backpack->nitems; ++i){
        if(!backpack->items[i]) continue;

        sum += items[i]->weight;
    }
    return sum;
}

// Calcula e retorna a utilidade dos itens na mochila
double getBackpackUtility(const Backpack *backpack, Item **items){
    double sum = 0.0f;
    for(int i = 0; i < backpack->nitems; ++i){
        if(!backpack->items[i]) continue;

        sum += items[i]->utility;
    }
    return sum;
}

void copyBackpack(Backpack *dest, Backpack* src){
    if(dest->items)
        free(dest->items);
    dest->items = (int*) malloc (sizeof(int) * src->nitems);
    memcpy(dest->items, src->items, src->nitems * sizeof(int));
    dest->nitems = src->nitems;
    dest->volume = src->volume;
}

// Libera a mochila da memória
void freeBackpack(Backpack *backpack){
    if(backpack->items)
        free(backpack->items);
    free(backpack);
}
