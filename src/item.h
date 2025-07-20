#ifndef ITEMS_H
#define ITEMS_H

typedef struct Item{
    double utility;
    double weight;
} Item;

typedef struct ItemsInfo{
    Item **items;
    int nitems;
} ItemsInfo;

Item* createItem(const double utility, const double weight);
void freeItemsInfo(ItemsInfo *items_info);

#endif
