#include "ag.h"
#include "build_csv.h"
#include <stdio.h>
#include <stdlib.h>

void solve(void){
    const int npop = 75;
    const int nger = 75;
    const int nelite = 7;
    const int ncromossomos = 2;

    const float pcruzamento = 1.0f;
    const float pmutacao = 0.01f;
    const float alfa = 0.75f;
    const float beta = 0.25f;


    AG *ag = criaAG(npop, nger, nelite, ncromossomos, pmutacao, pcruzamento,
                    alfa, beta);

    if(ag == NULL){
        printf("Erro ao criar o Algoritmo Genetico.\n");
        return;
    }

    criaPopulacaoInicial(ag);

    for(int geracao = 0; geracao < ag->nger; ++geracao){
        avaliaPopulacao(ag);
        int *vpais = roleta(ag);
        cruzamento(ag, vpais);
        mutacao(ag);
        elitismo(ag);

        copiaPopulacao(ag);

        // printPopulacao(ag);

        // Libera da memória o vetor de pais da geração atual
        free(vpais);
    }

    printResults(ag);

    destroiAG(ag);
}


void solveFactorialTest(const int npop, const int nger, const int nelite,
                        const int ncromossomos, const float pmutacao,
                        const float pcruzamento, const float alfa,
                        const float beta, FILE *results){
    AG *ag = criaAG(npop, nger, nelite, ncromossomos, pmutacao, pcruzamento,
                    alfa, beta);

    if(ag == NULL){
        printf("Erro ao criar o Algoritmo Genetico.\n");
        return;
    }

    criaPopulacaoInicial(ag);

    for(int geracao = 0; geracao < ag->nger; ++geracao){
        avaliaPopulacao(ag);
        int *vpais = roleta(ag);
        cruzamento(ag, vpais);
        mutacao(ag);
        elitismo(ag);

        copiaPopulacao(ag);

        // Libera da memória o vetor de pais da geração atual
        free(vpais);
    }

    printFinalResultsCSV(ag, results);

    destroiAG(ag);
}

void factorialTest(char *results_file_name){
    const int npop[] = {25, 50, 75, 100, 200};
    const int nger[] = {25, 50, 75, 100, 200};
    const int nelite[] = {0, 1, 2, 6, 10};
    const int ncromossomos[] = {2, 3, 4};

    const float pcruzamento[] = {1.0, 0.80, 0.60};
    const float pmutacao[] = {0.01, 0.05, 0.10};
    const float alfa[] = {0.75, 0.50, 0.25};
    const float beta[] = {0.25, 0.50, 0.75};

    FILE *results_fptr = createCSV(results_file_name);
    printFinalResultsIdfCSV(results_fptr);

    for(int ipop = 0; ipop < 5; ++ipop){
        for(int iger = 0; iger < 5; ++iger){
            for(int ielite = 0; ielite < 5; ++ielite){
                for(int icromossomos = 0; icromossomos < 3; ++icromossomos){
                    for(int ipcruzamento = 0; ipcruzamento < 3; ++ipcruzamento){
                        for(int ipmutacao = 0; ipmutacao < 3; ++ipmutacao){
                            for(int ialfa = 0; ialfa < 3; ++ialfa){
                                for(int ibeta = 0; ibeta < 3; ++ibeta){
                                    solveFactorialTest(npop[ipop], nger[iger],
                                          nelite[ielite],
                                          ncromossomos[icromossomos],
                                          pmutacao[ipmutacao],
                                          pcruzamento[ipcruzamento], alfa[ialfa],
                                          beta[ibeta], results_fptr);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    closeCSV(results_fptr);
}

void solveGenerationsTest(const int npop, const int nger, const int nelite,
                          const int ncromossomos, const float pmutacao,
                          const float pcruzamento, const float alfa,
                          const float beta, FILE *generation_results){
    AG *ag = criaAG(npop, nger, nelite, ncromossomos, pmutacao, pcruzamento,
                    alfa, beta);

    if(ag == NULL){
        printf("Erro ao criar o Algoritmo Genetico.\n");
        return;
    }

    criaPopulacaoInicial(ag);

    printGenIdfCSV(ag, generation_results);

    for(int geracao = 0; geracao < ag->nger; ++geracao){
        avaliaPopulacao(ag);
        int *vpais = roleta(ag);
        cruzamento(ag, vpais);
        mutacao(ag);
        elitismo(ag);

        copiaPopulacao(ag);

        printGenResultsCSV(ag, generation_results, geracao + 1);

        // Libera da memória o vetor de pais da geração atual
        free(vpais);
    }

    destroiAG(ag);
}

void generationsTest(char *generations_file_name){
    const int npop = 75;
    const int nger = 75;
    const int nelite = 7;
    const int ncromossomos = 2;

    const float pcruzamento = 1.00f;
    const float pmutacao = 0.01f;
    const float alfa = 0.75f;
    const float beta = 0.25f;

    FILE *generations_fptr = createCSV(generations_file_name);

    solveGenerationsTest(npop, nger, nelite, ncromossomos, pmutacao, pcruzamento,
                         alfa, beta, generations_fptr);

    closeCSV(generations_fptr);
}
