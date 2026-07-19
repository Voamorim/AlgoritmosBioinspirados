#ifndef AG_H
#define AG_H

#include "ag.h"
#include "backpack.h"
#include "item.h"
#include "utils.h"
#include "build_csv.h"
#include "max_heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AG{
    int npop; // Tamanho da população
    int ngen; // Quantidade de gerações
    double pcrossover; // Probabilidade de cruzamento
    double pmutation; // Probablidade de mutação
    int nelite; // Quantidade de elites
    int nitems; // Quantidade de itens

    double *fitness;
    Backpack **backpacks;
    Backpack **intermediate_backpacks;
    Item **items;

    double local_max;
    int count_gen_local_max;
    double extra_pmutation;
} AG;

AG* buildAG(const int npop, const int ngen, const double pcrossover,
             const double pmutation, const int nelite, const int nitems,
             const double backpack_volume, Item **items);
/**
 * Função: buildAG
 * Descrição: Inicializa a estrutura AG e atribui as informações do algoritmo
 *            genético.
 * Parâmetros:
 *      const int npop: quantidade de indivíduos a compor a população;
 *      const int ngen: quantidade de gerações do algoritmo genético;
 *      const double prossover: probabilidade de cruzamento entre um conjunto de
 *                              pais;
 *      const double pmutation: probabilidade de mutação para cada indivíduo da
 *                              população;
 *      const int nelite: quantidade de elites do algoritmo genético;
 *      const int nitems: quantidade de itens disponíveis a serem inseridos na
 *                        mochila;
 *      const double backpack_volume: volume de cada uma das mochilas;
 *      ****
 *
 * Retorno:
 *      (AG*) Retorna um ponteiro para a estrutura AG criada.
*/

void createInitialPopulation(AG *ag);
/**
 * Função: createInitialPopulation
 * Descrição: Cria a população inicial do algoritmo genético de forma aleatória.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

void evaluateFitness(AG *ag);
/**
 * Função: evaluateFitness
 * Descrição: Calcula os fitness dos indivíduos da população de acordo com a
 *            sua utilidade e a validade da solução.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

int* roulette(const AG *ag);
/**
 * Função: roulette
 * Descrição: Implementa uma roleta com os fitness dos indivíduos e constrói
 *            um vetor de pais para a próxima geração.
 * Parâmetros:
 *      const AG *ag: ponteiro para a estrutura contendo as informações do
 *                    algoritmo genético.
 * Retorna:
 *      (int*) Retorna um vetor de npop pais para a próxima geração.
*/

int rollTheRoulette(const double *prefix_sum, const int n);
/**
 * Função: rollTheRoulette
 * Descrição: Simula a operação de rodar uma roleta. Retorna o id do elemento
 *            selecionado pela operação.
 * Parâmetros:
 *      const double *prefix_sum: vetor contendo uma soma de prefixos dos
 *                                elementos a serem sorteados;
 *      const int n: quantidade de elementos no vetor de soma de prefixos.
 * Retorno:
 *      (int) Retorna o índice do elemento selecionado por rodar a roleta.
*/

void crossover(AG *ag, const int *parents);
/**
 * Função: crossover
 * Descrição: Realiza o cruzamento de 3 pontos dos indivíduos do algoritmo
 *            genético.
 *            A primeira parte do gene do filho pertence ao pai 1, a segunda
 *            ao pai 2, a terceira ao pai 1 e, por fim, o restante ao pai 2.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

void mutation(AG *ag);
/**
 * Função: mutation
 * Descrição: Realiza a mutação nos indivíduos do algoritmo genético.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

void elitism(AG *ag);
/**
 * Função: elitismo
 * Descrição: Realiza o elitismo do algoritmo genético, ou seja, copia os
 *            nelite indivíduos mais aptos da geração anterior para a próxima
 *            geração.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

void copyPopulation(AG *ag);
/**
 * Função: copyPopulation
 * Descrição: Copia todas as informações da população intermediária para a
 *            população principal.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

void freeAG(AG *ag);
/**
 * Função: freeAG
 * Descrição: Libera todas as informações da estrutura AG da memória.
 * Parâmetros:
 *      AG *ag: ponteiro para a estrutura contendo as informações do algoritmo
 *              genético.
 * Retorno:
 *      Não retorna nada.
*/

void printPopulation(const AG *ag);
/**
 * Função: printPopulation
 * Descrição: Imprime as informações de todos os indivíduos da população.
 * Parâmetros:
 *      const AG *ag: ponteiro para as informações do algoritmo genético.
 * Retorno:
 *      Não retorna nada.
*/

void printResults(const AG *ag);
/**
 * Função: printResults
 * Descrição: Imprime os resultados finais do algoritmo genético.
 * Parâmetros:
 *      const AG *ag: ponteiro para as informações do algoritmo genético.
 * Retorno:
 *      Não retorna nada.
*/

void printGenIdfCSV(const AG *ag, FILE *fptr);
/**
 * Função: printGenIdfCSV
 * Descrição: Imprime os identificadores das colunas das tabelas do arquivo CSV
 *            contendo as informações de cada geração de uma execução.
 * Parâmetros:
 *      const AG *ag: ponteiro para as informações do algoritmo genético;
 *      FILE *fptr: ponteiro para o arquivo a ter as informações escritas.
 * Retorno:
 *      Não retorna nada.
*/

void printGenResultsCSV(const AG *ag, FILE *fptr, const int generation);
/**
 * Função: printGenResultsCSV
 * Descrição: Imprime os resultados de uma única geração em um arquivo CSV.
 * Parâmetros:
 *      const AG *ag: ponteiro para as informações do algoritmo genético;
 *      FILE *fptr: ponteiro para o arquivo a ter as informações escritas;
 *      const int generation: identificador da geração atual.
 * Retorno:
 *      Não retorna nada.
*/

void printFinalResultsIdfCSV(FILE *fptr);
/**
 * Função: printFinalResultsIdfCSV
 * Descrição: Imprime os identificadores das colunas das tabelas do arquivo CSV
 *            contendo as informações finais de cada execução do algoritmo
 *            genético.
 * Parâmetros:
 *      FILE *fptr: ponteiro para o arquivo a ter as informações escritas.
 * Retorno:
 *      Não retrna nada.
*/

void printFinalResultsCSV(const AG *ag, FILE *fptr);
/**
 * Função: printFinalResultsCSV
 * Descrição: Imprime os resultados finais de uma execução do algoritmo genético
 *            em um arquivo CSV.
 * Parâmetros:
 *      const AG *ag: ponteiro para a estrutura contendo as informações do
 *                    algoritmo genético;
 *      FILE *fptr: ponteiro para o arquivo CSV a ter as informações escritas.
 * Retorno:
 *      Não retorna nada.
*/

#endif
