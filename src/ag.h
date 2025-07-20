#ifndef ALGORITMO_GENETICO
#define ALGORITMO_GENETICO

#include <stdio.h>

typedef struct AG {
    int npop; // Tamanho da população
    int nger; // Quantidade de gerações
    int nelite; // Quantidade de elementos para o elitismo
    int ncromossomos; // Quantidade de cromossomos por indivíduo

    float pmutacao; // Taxa de mutação
    float pcruzamento; // Probabilidade de cruzamento
    float alfa;
    float beta;

    double** pop;
    double** pop_intermediaria;

    double* fitness;
} AG;

AG* criaAG(int npop, int nger, int nelite, int ncromossomos,
           float pmutacao, float pcruzamento, float alfa, float beta);

void criaPopulacaoInicial(AG *ag);
void avaliaPopulacao(AG *ag);
double ackley(double x[], int n);

int *roleta(AG *ag);
/**
 * Determina os pais da nova geração pelo método de seleção por roleta
 *
 *
 * Retorno:
 *      - Vetor contendo npop pais da nova geração
*/

int rodaRoleta(const double roleta[], const int n);

void cruzamento(AG *ag, int* vpais);
void mutacao(AG *ag);
void elitismo(AG *ag);

void destroiAG(AG *ag);
void copiaPopulacao(AG *ag);

void printPopulacao(AG *ag);
void printResults(AG *ag);

void printGenIdfCSV(AG *ag, FILE *fptr);
void printGenResultsCSV(AG *ag, FILE *fptr, const int geracao);

void printFinalResultsIdfCSV(FILE *fptr);
void printFinalResultsCSV(AG *ag, FILE *fptr);

#endif
