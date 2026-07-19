#include "item.h"

#include <stdlib.h>

Item* createItem(const double utility, const double weight){
    Item* item = (Item*) malloc (sizeof(Item));
    item->utility = utility;
    item->weight = weight;
    return item;
}

void freeItemsInfo(ItemsInfo *items_info){
    for(int i = 0; i < items_info->nitems; ++i){
        free(items_info->items[i]);
    }
    free(items_info->items);
    free(items_info);
}
