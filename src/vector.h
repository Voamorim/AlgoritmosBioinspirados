#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

typedef struct Vector{
    double *data;
    int size;
    int capacity;
} Vector;

Vector* createVector(const int initial_capacity);
void vectorPushBack(Vector *v, const double value);
void vectorPopBack(Vector *v);
double vectorGet(const Vector *v, const int index);
void vectorSet(Vector *v, const int index, const double value);
int vectorSize(const Vector *v);
int vectorCapacity(const Vector *v);
void freeVector(Vector *v);

#endif
