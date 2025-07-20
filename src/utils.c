#include "utils.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int comparePairDoubleInt(const void *a, const void *b){
    const PairDoubleInt *pa = (const PairDoubleInt*) a;
    const PairDoubleInt *pb = (const PairDoubleInt*) b;

    if(pa->f > pb->f){
        return 1;
    } else {
        return -1;
    }
}

void sortVetPairDoubleInt(PairDoubleInt *v, const int n){
    qsort(v, n, sizeof(PairDoubleInt), comparePairDoubleInt);
}

int isEven(const int a){
    return !(a & 1);
}

void swapInt(int *a, int *b){
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

double minDouble(const double a, const double b){
    if(a < b) return a;
    return b;
}

double maxDouble(const double a, const double b){
    if(a >= b) return a;
    return b;
}

PairDoubleInt minDoubleVet(const double v[], const int n){
    if(n <= 0){
        printf("Erro ao encontrar o menor valor de double o vetor. O vetor tem"
               " tamanho n <= 0.\n");
        exit(1);
    }

    PairDoubleInt results;
    results.f = v[0];
    results.i = 0;

    for(int i = 1; i < n; ++i){
        if(v[i] < results.f){
            results.f = v[i];
            results.i = i;
        }
    }

    return results;
}

PairDoubleInt maxDoubleVet(const double v[], const int n){
    if(n <= 0){
        printf("Erro ao encontrar o maior valor de double no vetor. O vetor tem"
               " tamanho n <= 0.\n");
        exit(1);
    }

    PairDoubleInt results;
    results.f = v[0];
    results.i = 0;

    for(int i = 0; i < n; ++i){
        if(v[i] > results.f){
            results.f = v[i];
            results.i = i;
        }
    }

    return results;
}

double sumVetDouble(const double v[], const int n){
    double sum = 0.0f;
    for(int i = 0; i < n; ++ i){
        sum += v[i];
    }
    return sum;
}

double* accumulateVetDouble(const double v[], const int n){
    double* prefix_sum = (double*) calloc(n + 1, sizeof(double));

    if(!prefix_sum){
        printf("Erro ao alocar memoria para o array de prefixos.\n");
        exit(1);
    }

    for(int i = 1; i <= n; ++i){
        prefix_sum[i] = prefix_sum[i-1] + v[i-1];
    }

    return prefix_sum;
}

// Gera e retorna um número aleátorio em um intervalo arbitrário
double randInRange(const double low, const double high){
    double delta = high - low;

    // Obtém um número aleatório no intervalo [0, delta]
    double r = ((double) rand() / RAND_MAX) * delta;

    // Incrementa o valor aleatório gerado por low, para que o resultado se
    // encontre no intervalo [low, high]
    return r + low;
}

// Calcula e retorna a quantidade de caracteres necessária para representar
// um valor do tipo inteiro em uma string
int getIntLen(const int n){
    return (int) snprintf(NULL, 0, "%d", n) + 1;
}

// Calcula e retorna a quantidade de caracteres necessária para representar
// um valor do tipo double em uma string
int getDoubleLen(const double n){
    return (int) snprintf(NULL, 0, "%.2lf", n) + 1;
}

// Transforma e retorna um valor do tipo int em uma string
char* intToString(const int n, const int len){
    char *string = (char*) malloc (sizeof(char) * len);
    sprintf(string, "%d", n);
    return string;
}

// Transforma e retorna um valor do tipo double em uma string
char* doubleToString(const double n, const int len){
    char *string = (char*) malloc (sizeof(char) * len);
    sprintf(string, "%.2lf", n);
    return string;
}

// Calcula e retorna a média dos elementos de um vetor de elementos do tipo double
double getVetAverage(const double *v, const int n){
    return sumVetDouble(v, n) / (double) n;
}

char* getDynamicAllocatedString(const char *str){
    int str_len = strlen(str) + 1;
    char *new_string = (char*) malloc (sizeof(char) * str_len);
    strcpy(new_string, str);
    return new_string;
}
