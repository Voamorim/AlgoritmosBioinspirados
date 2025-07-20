#include "io.h"
#include "vector.h"
#include "item.h"

#include <stdlib.h>
#include <string.h>

FILE* getUtilityFile(const int argc, char **argv){
    char utility_file_name[100] = "../inputs/p08_p.txt";
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-u")){
            strcpy(utility_file_name, argv[i+1]);
            strcat("../input/", utility_file_name);
            break;
        }
    }
    return fopen(utility_file_name, "r");
}

FILE* getWeightsFile(const int argc, char **argv){
    char weights_file_name[100] = "../inputs/p08_w.txt";
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-w")){
            strcpy(weights_file_name, argv[i+1]);
            strcat("../input/", weights_file_name);
            break;
        }
    }
    return fopen(weights_file_name, "r");
}

FILE* getBackpackFile(const int argc, char **argv){
    char backpack_file_name[100] = "../inputs/p08_c.txt";
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-b")){
            strcpy(backpack_file_name, argv[i+1]);
            strcat("../input/", backpack_file_name);
            break;
        }
    }
    return fopen(backpack_file_name, "r");
}

int getSolutionId(const int argc, char **argv){
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-s")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

double readBackpackVolume(FILE *fptr){
    double capacity = 0;
    if(fscanf(fptr, "%lf", &capacity) == 1){
        fclose(fptr);
        return capacity;
    }
    printf("Erro ao ler a capacidade da mochila do arquivo.\n");
    exit(1);
}

Vector* readItemsUtility(FILE *fptr){
    if(fptr == NULL){
        printf("Erro: arquivo de utilidade dos itens igual a NULL.\n");
        exit(1);
    }

    Vector* v = createVector(10);

    if(v == NULL){
        return NULL;
    }

    double value = 0.0f;
    while(fscanf(fptr, "%lf", &value) == 1){
        vectorPushBack(v, value);
    }

    fclose(fptr);
    return v;
}

Vector* readItemsWeight(FILE *fptr){
    if(fptr == NULL){
        printf("Erro: arquivo de pesos dos itens igual a NULL.\n");
        exit(1);
    }

    Vector *v = createVector(10);

    if(v == NULL){
        return NULL;
    }

    double value = 0.0;
    while(fscanf(fptr, "%lf", &value) == 1){
        vectorPushBack(v, value);
    }

    fclose(fptr);
    return v;
}

ItemsInfo* readItems(FILE *fptr_items_weight, FILE *fptr_items_utility){
    Vector* weights = readItemsWeight(fptr_items_weight);
    Vector* utilities = readItemsUtility(fptr_items_utility);

    Item **items = (Item**) malloc (sizeof(Item*) * weights->size);

    for(int i = 0; i < weights->size; ++i){
        items[i] = createItem(vectorGet(utilities, i), vectorGet(weights, i));
    }

    ItemsInfo *items_info = (ItemsInfo*) malloc (sizeof(ItemsInfo));
    items_info->items = items;
    items_info->nitems = weights->size;

    freeVector(weights);
    freeVector(utilities);

    return items_info;
}
