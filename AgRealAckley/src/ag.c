#include "ag.h"
#include "utils.h"
#include "min_heap.h"
#include "build_csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define XMIN -2.0
#define XMAX 2.0

AG* criaAG(int npop, int nger, int nelite, int ncromossomos, float pmutacao,
           float pcruzamento, float alfa, float beta){
    // Verifica se os parâmetros são válidos
    if(nelite > npop){
        perror("Erro. Parametros invalidos. (nelite > npop).\n");
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
    ag->nelite = isEven(npop) ^ isEven(nelite) ? nelite + 1 : nelite;
    ag->ncromossomos = ncromossomos;

    ag->pmutacao = pmutacao;
    ag->pcruzamento = pcruzamento;
    ag->alfa = alfa;
    ag->beta = beta;

    ag->pop = (double**) malloc (npop * sizeof(double*));

    // Verifica se as linhas da matriz foram alocadas corretamente
    if(ag->pop == NULL){
        perror("Erro ao alocar as linhas da matriz de população.");
        free(ag);
        return NULL;
    }

    for(int i = 0; i < ag->npop; ++i){
        ag->pop[i] = (double*) malloc (ncromossomos * sizeof(double));

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

    ag->pop_intermediaria = (double**) malloc (npop * sizeof(double*));

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
        ag->pop_intermediaria[i] = (double*) malloc (ncromossomos * sizeof(double));

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
    ag->fitness = (double*) malloc (sizeof(double) * ag->npop);

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
    for(int i = 0; i < ag->npop; ++i){
        for(int j = 0; j < ag->ncromossomos; ++j){

            double delta = XMAX - XMIN;

            double r = (double) rand() / RAND_MAX;

            ag->pop[i][j] = XMIN + r * delta;
        }
    }
}

int* roleta(AG *ag){
    int* vpais = (int*) malloc(sizeof(int) * ag->npop);

    double *fit_roleta = (double*) malloc (sizeof(double) * ag->npop);

    for(int i = 0; i < ag->npop; ++i){
        if(ag->fitness[i])
            fit_roleta[i] = 1.0 / ag->fitness[i];
        else {
            fit_roleta[i] = 1.0 / 1e-12;
        }
    }

    double *roleta = accumulateVetDouble(fit_roleta, ag->npop);
    int n_roleta = ag->npop + 1;


    for(int i = 0; i < ag->npop; ++i){
        vpais[i] = rodaRoleta(roleta, n_roleta) - 1;
    }

    free(roleta);
    free(fit_roleta);

    return vpais;
}

int rodaRoleta(const double roleta[], const int n){
    double r = ((double) rand() / RAND_MAX) * roleta[n-1];

    int low = 0, high = n - 1;

    while(low < high){
        int mid = (low + high) / 2;


        if(r < roleta[mid]){
            high = mid;
        } else{
            low = mid + 1;
        }
    }
    return high;
}

void avaliaPopulacao(AG *ag){
    // Calcula o fitness de toda a população
    for(int i = 0; i < ag->npop; ++i){
        ag->fitness[i] = ackley(ag->pop[i], ag->ncromossomos);
    }
}

double ackley(double x[], int n){
    double sum_1 = 0.0f;
    double sum_2 = 0.0f;

    // Calcula o valor dos somatórios
    for(int i = 0; i < n; ++i){
        sum_1 += pow(x[i], 2);
        sum_2 += cos(2.0 * M_PI * x[i]);
    }

    // Retorna o resultado da função para os parâmetros passados
    return -20.0 * exp(-0.2 * sqrt(1.0/n * sum_1))
           - exp(1.0/n * sum_2)
           + 20.0 + M_E;
}

void cruzamento(AG *ag, int* vpais){
    int i = 0;
    for(i = ag->nelite; i < ag->npop - 1; i+= 2){
        int pai_1 = vpais[i];
        int pai_2 = vpais[i+1];

        double r = (double) rand() / RAND_MAX;

        // Caso o cruzamento não deva ocorrer, apenas copia os cromossomos dos
        // pais para a nova geração
        if(r > ag->pcruzamento){
            memcpy(ag->pop_intermediaria[i], ag->pop[pai_1],
                   ag->ncromossomos * sizeof(double));
            memcpy(ag->pop_intermediaria[i+1], ag->pop[pai_2],
                   ag->ncromossomos * sizeof(double));
            continue;
        }

        for(int j = 0; j < ag->ncromossomos; ++j){
            double x = ag->pop[pai_1][j];
            double y = ag->pop[pai_2][j];

            double d = fabs(x - y);

            double low, high;
            if(x <= y){
                low = x - ag->alfa * d;
                high = y + ag->beta * d;
            } else {
                low = y - ag->beta * d;
                high = x + ag->alfa * d;
            }

            double cfilho_1 = randInRange(low, high);
            double cfilho_2 = randInRange(low, high);

            ag->pop_intermediaria[i][j] = cfilho_1;
            ag->pop_intermediaria[i+1][j] = cfilho_2;
        }
    }
}

void mutacao(AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        for(int j = 0; j < ag->ncromossomos; ++j){
            // Calcula a probabilidade de ocorrer a mutação
            double r = (double) rand() / RAND_MAX;

            // Caso o valor gerado seja menor ou igual à probabilidade de
            // mutação, o cromossomo recebe um valor aleatório dentro do
            // intervalo definido
            if(r <= ag->pmutacao){
                ag->pop_intermediaria[i][j] = randInRange(XMIN, XMAX);
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

        // Copia os cromossomos do indivíduo para a população intermediária
        memcpy(ag->pop_intermediaria[i], ag->pop[individuo.id],
               ag->ncromossomos * sizeof(double));
    }

    // Libera a memória da min_heap
    destroiMinHeap(heap);
}

void destroiAG(AG *ag){
    // Libera as dimensões dos vetores de população
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
               ag->ncromossomos * sizeof(double));
    }
}

void printPopulacao(AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        printf("Individuo %d: \n", i);
        printf("\tCromossomos: ");
        for(int j = 0; j < ag->ncromossomos; ++j){
            printf("%.10lf ", ag->pop[i][j]);
        }
        printf("\n");
        printf("\tFitness: %f\n", ag->fitness[i]);
        printf("\n");
    }
}

void printResults(AG *ag){
    PairDoubleInt r = minDoubleVet(ag->fitness, ag->npop);

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("                  Resumo dos Resultados                    \n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf(" Menor valor: %.16f                                        \n", r.f);
    printf(" Cromossomos: ");

    for(int i = 0; i < ag->ncromossomos; ++i){
        printf("%.16lf ", ag->pop[r.i][i]);
    }
    printf("\n");

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void printGenIdfCSV(AG *ag, FILE *fptr){

    const int n_contents = ag->npop + 1;

    char **contents = (char**) malloc (sizeof(char*) * n_contents);

    // Inserindo a coluna que indica as gerações no vetor
    contents[0] = (char*) malloc (sizeof(char) * (strlen("Geracao") + 1));
    if(contents[0] == NULL){
        printf("Erro ao alocar memoria para o vetor de strings a serem escritas"
               " no arquivo .csv.\n");
        return;
    }
    contents[0] = getDynamicAllocatedString("Geracao");

    // Incluindo as colunas representando os individuos
    for(int i = 1; i <= ag->npop; ++i){
        int idv_len = getIntLen(i) + strlen("Individuo ") + 1;
        contents[i] = (char*) malloc (sizeof(char) * idv_len);
        if(contents[i] == NULL){
            printf("Erro ao alocar memoria para o vetor de strings a serem "
                   "escritas no arquivo .csv.\n");
            return;
        }
        strcpy(contents[i], "Individuo ");

        char *str = intToString(i, getIntLen(i));
        strcat(contents[i], str);
        free(str);
    }

    // Escreve os conteúdos no arquivo .csv
    writeLineCSV(fptr, contents, n_contents);

    // Liberando o vetor de strings da memória
    for(int i = 0; i < n_contents; ++i)
        free(contents[i]);
    free(contents);
}

void printGenResultsCSV(AG *ag, FILE *fptr, const int geracao){
    int n_values = ag->npop + 1;
    char **values = (char**) malloc (sizeof(char*) * n_values);

    // Inserindo a geração na primeira posição do vetor
    int gen_len = getIntLen(geracao);
    values[0] = (char*) malloc (gen_len * sizeof(char));
    if(values[0] == NULL){
        printf("Erro ao alocar memoria para o vetor de strings a serem escritas"
               " no arquivo .csv.\n");
        free(values);
        return;
    }
    char *geracao_str = intToString(geracao, gen_len);
    strcpy(values[0], geracao_str);
    free(geracao_str);

    // Incluindo os fitness dos indivíduos
    for(int i = 0; i < ag->npop; ++i){
        int fit_len = getDoubleLen(ag->fitness[i]);
        values[i+1] = (char*) malloc (fit_len * sizeof(char));
        if(values[i+1] == NULL){
            printf("Erro ao alocar memoria para o vetor de strings a serem "
                   "escritas no arquivo .csv.\n");
            return;
        }
        char *fit_str = doubleToString(ag->fitness[i], fit_len);
        strcpy(values[i+1], fit_str);
        free(fit_str);
    }

    // Escreve os dados da geração no arquivo CSV
    writeLineCSV(fptr, values, n_values);

    // Liberando o vetor de strings da memória
    for(int i = 0; i < n_values; ++i)
        free(values[i]);
    free(values);
}

void printFinalResultsIdfCSV(FILE *fptr){
    const int n_contents = 10;

    char **contents = (char**) malloc (sizeof(char*) * n_contents);

    // Inserindo a coluna de probabilidade de cruzamento
    contents[0] = getDynamicAllocatedString("Probabilidade de Cruzamento");

    // Inserindo a coluna de quantidade de elites
    contents[1] = getDynamicAllocatedString("Elites");

    // Inserindo a coluna de probabilidade de mutação
    contents[2] = getDynamicAllocatedString("Probabilidade de Mutacao");

    // Inserindo a coluna de valores de alfa
    contents[3] = getDynamicAllocatedString("Alfa");

    // Inserindo a coluna de valores de beta
    contents[4] = getDynamicAllocatedString("Beta");

    // Inserindo a coluna de quantidade de cromossomos
    contents[5] = getDynamicAllocatedString("Numero de Cromossomos");

    // Inserindo a coluna de número de gerações
    contents[6] = getDynamicAllocatedString("Geracoes");

    // Inserindo a coluna de tamanho da população
    contents[7] = getDynamicAllocatedString("Tamanho da Populacao");

    // Inserindo a coluna de fitness do indivíduo de melhor aptidão
    contents[8] = getDynamicAllocatedString("Individuo de Melhor Aptidao");

    // Inserindo a coluna de aptidão média dos indivíduos
    contents[9] = getDynamicAllocatedString("Aptidao Media");

    // Escrevendo os identificadores das colunas no arquivo .csv
    writeLineCSV(fptr, contents, n_contents);

    // Liberando as strings da memória
    for(int i = 0; i < n_contents; ++i)
        free(contents[i]);
    free(contents);
}

void printFinalResultsCSV(AG *ag, FILE *fptr){
    int n_values = 10;
    char **values = (char**) malloc (sizeof(char*) * n_values);

    // Inserindo as taxas de cruzamento
    values[0] = doubleToString(ag->pcruzamento, getDoubleLen(ag->pcruzamento));

    // Inserindo a quantidade de elites
    values[1] = intToString(ag->nelite, getIntLen(ag->nelite));

    // Inserindo a probabilidade de mutação
    values[2] = doubleToString(ag->pmutacao, getDoubleLen(ag->pmutacao));

    // Inserindo o valor de alfa
    values[3] = doubleToString(ag->alfa, getDoubleLen(ag->alfa));

    // Inserindo o valor de beta
    values[4] = doubleToString(ag->beta, getDoubleLen(ag->beta));

    // Inserindo a taxa de cruzamento
    values[5] = intToString(ag->ncromossomos, getIntLen(ag->ncromossomos));

    // Inserindo o número de gerações
    values[6] = intToString(ag->nger, getIntLen(ag->nger));

    // Inserindo o tamanho da população
    values[7] = intToString(ag->npop, getIntLen(ag->npop));

    // Inserindo o fitness do indivíduo de melhor aptidão
    PairDoubleInt best_fit = minDoubleVet(ag->fitness, ag->npop);
    int best_fit_len = getDoubleLen(best_fit.f);
    values[8] = doubleToString(best_fit.f, best_fit_len);

    // Inserindo a aptidão média dos indivíduos
    double average = getVetAverage(ag->fitness, ag->npop);
    int average_len = getDoubleLen(average);
    values[9] = doubleToString(average, average_len);

    // Escreve os conteúdos no arquivo .csv
    writeLineCSV(fptr, values, n_values);

    // Liberando o vetor de strings da memória
    for(int i = 0; i < n_values; ++i)
        free(values[i]);
    free(values);
}
