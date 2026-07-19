#include "ag.h"
#include "utils.h"
#include "min_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

AG* criaAG(int npop, int nger, int nelite, int ngenes, int ncromossomos,
           float pmutacao, float pcruzamento){
    // Verifica se os parâmetros são válidos
    if(nelite > npop || ncromossomos >= 32){
        perror("Erro. Parametros invalidos. (nelite > npop or ncromossomos"
               ">= 32).\n");
        return NULL;
    }

    AG *ag = (AG*) malloc (sizeof(AG));

    // Interrompe o programa caso a variável não seja alocada com sucesso
    if(ag == NULL){
        perror("Erro ao alocar a variavel AG dinamicamente.");
        return NULL;
    }

    ag->npop = npop;
    ag->nger = nger;
    ag->nelite = nelite;
    ag->ngenes = ngenes;
    ag->ncromossomos = ncromossomos;

    ag->pmutacao = pmutacao;
    ag->pcruzamento = pcruzamento;

    ag->pop = (int**) malloc (npop * sizeof(int*));

    // Verifica se as linhas da matriz foram alocadas corretamente
    if(ag->pop == NULL){
        perror("Erro ao alocar as linhas da matriz de população.");
        free(ag);
        return NULL;
    }

    for(int i = 0; i < ag->npop; ++i){
        ag->pop[i] = (int*) malloc (ngenes * ncromossomos * sizeof(int));

        // Verifica se os genes foram alocados corretamente
        if(ag->pop[i] == NULL){
            perror("Erro ao alocar memória dos genes da população.");

            // Liberando as memórias já alocadas
            for(int j = 0; j < i; ++j){
                free(ag->pop[j]);
            }
            free(ag->pop);
            free(ag);
            return NULL;
        }
    }

    ag->pop_intermediaria = (int**) malloc (npop * sizeof(int*));

    // Verifica se as linhas da matriz foram alocadas corretamente
    if(ag->pop_intermediaria == NULL){
        perror("Erro ao alocar as linhas da matriz de população intermediária.");

        // Liberando o vetor de população
        for(int i = 0; i < ag->npop; ++i){
            free(ag->pop[i]);
        }
        free(ag->pop);

        free(ag);
        return NULL;
    }

    for(int i = 0; i < ag->npop; ++i){
        ag->pop_intermediaria[i] = (int*) malloc (ngenes * ncromossomos * sizeof(int));

        // Verifica se os genes foram alocados corretamente
        if(ag->pop_intermediaria[i] == NULL){
            perror("Erro ao alocar memória dos genes da população intermediária.");

            // Liberando as memórias já alocadas
            for(int j = 0; j < i; ++j){
                free(ag->pop_intermediaria[j]);
            }
            free(ag->pop_intermediaria);

            // Liberando as memórias alocadas da população original
            for(int i = 0; i < ag->npop; ++i){
                free(ag->pop[i]);
            }
            free(ag->pop);

            free(ag);
            return NULL;
        }
    }

    // Aloca o vetor de fitness
    ag->fitness = (float*) malloc (sizeof(float) * ag->npop);

    // Verifica se o vetor de fitness foi alocado corretamente
    if(ag->fitness == NULL){
        perror("Erro ao alocar o vetor de fitness.");

        // Liberando a população original e a intermediária
        for(int i = 0; i < ag->npop; ++i){
            free(ag->pop[i]);
            free(ag->pop_intermediaria[i]);
        }
        free(ag->pop);
        free(ag->pop_intermediaria);
        free(ag);

        return NULL;
    }

    return ag;
}

void criaPopulacaoInicial(AG *ag){
    srand(time(NULL));

    for(int i = 0; i < ag->npop; ++i){
        for(int j = 0; j < ag->ncromossomos * ag->ngenes; ++j){
            int value = rand() % 2;

            ag->pop[i][j] = value;
        }
    }
}

int* torneio(AG *ag){
    int* vpais = (int*) malloc (sizeof(int) * ag->npop);
    float pv = 0.9; // Probabilidade de vitória do melhor

    for(int i = 0; i < ag->npop; ++i){
        // Seleciona dois pais aleatórios
        int pai_1 = rand() % ag->npop;
        int pai_2 = rand() % ag->npop;

        // Impede com que sejam selecionados os mesmos pais
        while(pai_1 == pai_2){
            pai_2 = rand() % ag->npop;
        }

        // Gera a probabilidade do melhor ser vencedor
        float r = (float)rand() / RAND_MAX;

        int vencedor;

        if(ag->fitness[pai_1] > ag->fitness[pai_2]){
            vencedor = pai_2;
            if(r > pv){
                vencedor = pai_1;
            }
        } else {
            vencedor = pai_1;
            if(r > pv){
                vencedor = pai_2;
            }
        }

        vpais[i] = vencedor;
    }

    return vpais;
}

void avaliaPopulacao(AG *ag){
    const float xmin = -2;
    const float xmax = 2;

    float x[ag->npop][ag->ncromossomos];

    for(int i = 0; i < ag->npop; ++i){
        for(int j = 0; j < ag->ncromossomos; ++j){
            x[i][j] = binToFloat(ag->pop[i] + j * ag->ngenes,
                                 ag->ngenes, xmin, xmax);
        }
    }


    for(int i = 0; i < ag->npop; ++i){
        ag->fitness[i] = ackley(x[i], ag->ncromossomos);
    }
}

float ackley(float x[], int n){
    float sum_1 = 0.0f;
    float sum_2 = 0.0f;

    for(int i = 0; i < n; ++i){
        sum_1 += pow(x[i], 2);
        sum_2 += cos(2.0 * M_PI * x[i]);
    }

    return -20.0 * exp(-0.2 * sqrt(1.0/n * sum_1))
           - exp(1.0/n * sum_2)
           + 20.0 + M_E;
}

void cruzamento(AG *ag, int* vpais){
    for(int i = 0; i < ag->npop - 1; i+= 2){
        float r = (float) rand() / RAND_MAX;

        int pai_1 = vpais[i];
        int pai_2 = vpais[i+1];

        // Caso o cruzamento não deva ocorrer, apenas copia os genes dos pais
        if(r > ag->pcruzamento){
            memcpy(ag->pop_intermediaria[i], ag->pop[pai_1],
                   ag->ngenes * ag->ncromossomos * sizeof(int));
            memcpy(ag->pop_intermediaria[i+1], ag->pop[pai_2],
                   ag->ngenes * ag->ncromossomos * sizeof(int));
            continue;
        }

        for(int j = 0; j < ag->ncromossomos; ++j){
            // Escolhe um ponto de corte entre 0 e o número de genes
            int ponto_de_corte = rand() % ag->ngenes;

            for(int k = 0; k < ag->ngenes; ++k){
                int gene_id = j * ag->ngenes + k;
                if(k < ponto_de_corte){
                    ag->pop_intermediaria[i][gene_id] = ag->pop[pai_1][gene_id];
                    ag->pop_intermediaria[i+1][gene_id] = ag->pop[pai_2][gene_id];
                } else {
                    ag->pop_intermediaria[i][gene_id] = ag->pop[pai_2][gene_id];
                    ag->pop_intermediaria[i+1][gene_id] = ag->pop[pai_1][gene_id];
                }
            }
        }
    }
}

void mutacao(AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        for(int j = 0; j < ag->ncromossomos; ++j){
            for(int k = 0; k < ag->ngenes; ++k){
                // Gera um número aleatório entre 0 e 1
                float r = (float) rand() / RAND_MAX;

                int gen_id = j * ag->ngenes + k;
                // Verifica se o número gerado é menor que a probabilidade de mutação
                if(r < ag->pmutacao){
                    ag->pop_intermediaria[i][gen_id] = 1 ^ ag->pop_intermediaria[i][gen_id];
                }
            }
        }
    }
}

void elitismo(AG *ag){
    MinHeap *heap = constroiMinHeap(ag->fitness, ag->npop);

    for(int i = 0; i < ag->nelite; ++i){
        // Extrai as informações do indivíduo de menor fitness da população
        // (minimização)
        No individuo = extrairMinHeap(heap);

        // Copia os genes do indivíduo para a população intermediária
        memcpy(ag->pop_intermediaria[i], ag->pop[individuo.id],
               ag->ngenes * ag->ncromossomos * sizeof(int));
    }

    // Libera a memória da min_heap
    destroiMinHeap(heap);
}

void destroiAG(AG *ag){
    // Libera as matrizes de população
    for(int i = 0; i < ag->npop; ++i){
        free(ag->pop[i]);
        free(ag->pop_intermediaria[i]);
    }

    free(ag->pop);
    free(ag->pop_intermediaria);
    free(ag->fitness);

    free(ag);
}

void copiaPopulacao(AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        memcpy(ag->pop[i], ag->pop_intermediaria[i],
               ag->ngenes * ag->ncromossomos * sizeof(int));
    }
}

void printPopulacao(AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        printf("Individuo %d: \n", i);
        printf("\tGenes: ");
        for(int j = 0; j < ag->ncromossomos; ++j){
            for(int k = 0; k < ag->ngenes; ++k)
                printf("%d", ag->pop[i][j * ag->ngenes + k]);
            printf(" ");
        }
        printf("\n");
        printf("\tFitness: %f\n", ag->fitness[i]);
        printf("\n");
    }
}

void printResults(AG *ag){
    PairFloatInt r = minFloatVet(ag->fitness, ag->npop);

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("                  Resumo dos Resultados                    \n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf(" Menor valor: %.10f                                        \n", r.f);
    printf(" Cromossomos: ");

    for(int i = 0; i < ag->ncromossomos; ++i){
        for(int j = 0; j < ag->ngenes; ++j){
            printf("%d", ag->pop[r.i][i * ag->ngenes + j]);
        }
        printf(" ");
    }
    printf("\n");

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}
