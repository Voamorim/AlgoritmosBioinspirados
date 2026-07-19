#include "utils.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int binToUnsignedInt(int* bin, int n){
    unsigned int v = 0;

    for(int i = 0; i < n; ++i){
        v = (v << 1) | (bin[i] & 1);
    }

    return v;
}

float binToFloat(int* bin, int n, float xmin, float xmax){
    unsigned int int_value = binToUnsignedInt(bin, n);

    float real_value = xmin + ((xmax - xmin) / ((float) (1U << n) - 1)) * int_value;

    return real_value;
}

void swapInt(int *a, int *b){
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

float minFloat(const float a, const float b){
    if(a < b) return a;
    return b;
}

PairFloatInt minFloatVet(float v[], const int n){
    if(n <= 0){
        printf("Erro ao encontrar o menor valor de float no vetor. O vetor tem"
               " tamanho n <= 0.\n");
        exit(1);
    }

    PairFloatInt results;
    results.f = v[0];
    results.i = 0;

    for(int i = 0; i < n; ++i){
        if(v[i] < results.f){
            results.f = v[i];
            results.i = i;
        }
    }

    return results;
}
