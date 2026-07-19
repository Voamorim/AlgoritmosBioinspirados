#include "vector.h"
#include <stdlib.h>

Vector* createVector(const int initial_capacity){
    Vector *v = (Vector*) malloc (sizeof(Vector));

    if(v == NULL){
        printf("Erro ao alocar memoria para o Vector.\n");
        return NULL;
    }

    v->size = 0;
    v->capacity = initial_capacity;

    v->data = (double*) malloc (sizeof(double) * initial_capacity);

    if(v->data == NULL){
        printf("Erro ao alocar memoria para os elementos do Vector.\n");
        return NULL;
    }
    return v;
}

void vectorPushBack(Vector *v, const double value){
    if(v->size == v->capacity){
        v->capacity = v->capacity * 2 > 0 ? v->capacity * 2 : 1;
        v->data = (double*) realloc (v->data, v->capacity * sizeof(double));
        if(v->data == NULL){
            printf("Erro ao realocar memoria para os elementos do vector.\n");
            return;
        }
    }
    v->data[v->size] = value;
    v->size += 1;
}

void vectorPopBack(Vector *v){
    if(v->size < 1){
        printf("Erro ao remover elemento ao final do vector. Nao existem "
               "elementos no vector.\n");
        return;
    }

    v->size -= 1;
}

double vectorGet(const Vector *v, const int index){
    if(index >= v->size){
        printf("Erro: indice fora dos limites do vector.\n");
        exit(1);
    }
    return v->data[index];
}

void vectorSet(Vector *v, const int index, const double value){
    if(index >= v->size){
        printf("Erro: indice fora dos limites do vector.\n");
        exit(1);
    }
    v->data[index] = value;
}

int vectorSize(const Vector *v){ return v->size; }
int vectorCapacity(const Vector *v){ return v->capacity; }

void freeVector(Vector *v){
    free(v->data);
    free(v);
}

