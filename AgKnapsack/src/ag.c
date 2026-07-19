#include "ag.h"

AG* buildAG(const int npop, const int ngen, const double pcrossover,
             const double pmutation, const int nelite, const int nitems,
             const double backpack_volume, Item **items){
    AG* ag = (AG*) malloc (sizeof(AG));

    // Inicializa os parâmetros do algoritmo genético
    ag->npop = npop;
    ag->ngen = ngen;
    ag->pcrossover = pcrossover;
    ag->pmutation = pmutation;
    ag->nelite = isEven(nelite) ? nelite : nelite + 1;
    ag->nitems = nitems;

    // Informações para incrementar a taxa de mutação quando houver estagnação
    ag->local_max = 0;
    ag->count_gen_local_max = 0;
    ag->extra_pmutation = 0.0;

    ag->backpacks = (Backpack**) malloc (sizeof(Backpack*) * npop);

    // Verifica se o vetor de mochilas foi alocado com sucesso
    if(!ag->backpacks){
        printf("Erro ao alocar memoria para o vetor de mochilas.\n");
        free(ag);
        return NULL;
    }

    // Aloca as mochilas para o vetor de mochilas
    for(int idv = 0; idv < npop; ++idv){
        ag->backpacks[idv] = createBackpack(nitems, backpack_volume);

        // Verifica se a mochila foi alocada com sucesso
        if(!ag->backpacks[idv]){
            printf("Erro ao alocar memoria para mochila.\n");
            for(int i = 0; i < idv; ++i){
                freeBackpack(ag->backpacks[i]);
            }
            free(ag->backpacks);
            free(ag);
            return NULL;
        }
    }

    // Vetor para armazenar os itens disponíveis
    ag->items = items;

    ag->fitness = (double*) malloc (sizeof(double) * npop);

    // Verifica se o vetor de fitness foi alocado com sucesso
    if(!ag->fitness){
        printf("Erro ao alocar memoria para o vetor de fitness.\n");
        for(int i = 0; i < ag->npop; ++i){
            freeBackpack(ag->backpacks[i]);
        }
        free(ag->backpacks);
        free(ag);
        return NULL;
    }

    ag->intermediate_backpacks = (Backpack**) malloc (sizeof(Backpack*) * npop);

    if(!ag->intermediate_backpacks){
        printf("Erro ao alocar memoria para o vetor de mochilas intermediarias.\n");
        for(int i = 0; i < ag->npop; ++i){
            freeBackpack(ag->backpacks[i]);
        }
        free(ag->backpacks);
        free(ag->fitness);
        free(ag);
        return NULL;
    }

    for(int idv = 0; idv < ag->npop; ++idv){
        ag->intermediate_backpacks[idv] = createBackpack(nitems, backpack_volume);

        // Verifica se a mochila foi alocada com sucesso
        if(!ag->intermediate_backpacks[idv]){
            printf("Erro ao alocar memoria para a mochila.\n");
            for(int i = 0; i < idv; ++i){
                freeBackpack(ag->intermediate_backpacks[i]);
            }
            free(ag->intermediate_backpacks);
            for(int i = 0; i < ag->npop; ++i){
                freeBackpack(ag->backpacks[i]);
            }
            free(ag->backpacks);
            free(ag->fitness);
            free(ag);
            return NULL;
        }
    }

    return ag;
}

void createInitialPopulation(AG *ag){
    // Inicializa as mochilas da população com itens aleatórios
    for(int i = 0; i < ag->npop; ++i){
        assignRandomItemsToBackpack(ag->backpacks[i]);
    }
}

void evaluateFitness(AG *ag){
    // Calcula e atualiza o fitness de todos os indivíduos da população de acordo
    // com a utilidade e a validade da solução
    for(int b = 0; b < ag->npop; ++b){
        double items_weight = getBackpackUsedWeight(ag->backpacks[b],
                                                    ag->items);

        // Caso seja uma solução válida, o fitness é igual à soma das utilidades
        if(items_weight <= ag->backpacks[b]->volume){
            ag->fitness[b] = getBackpackUtility(ag->backpacks[b],
                                                ag->items);
        } else {
        // Caso contrário, o fitness da solução sofre uma penalização
            double utility_sum = getBackpackUtility(ag->backpacks[b],
                                                    ag->items);

            double pweights_sum = 0.0;
            for(int i = 0; i < ag->nitems; ++i){
                pweights_sum += ag->items[i]->weight ;
            }

            ag->fitness[b] = utility_sum - (utility_sum * (pweights_sum- ag->backpacks[b]->volume));
        }
    }
}

int* roulette(const AG* ag){
    int *vparents = (int*) malloc (sizeof(int) * ag->npop);

    PairDoubleInt* sorted_fitness = (PairDoubleInt*) malloc
                                            (sizeof(PairDoubleInt) * ag->npop);

    // Insere os valores de fitness e seus índices no vetor
    for(int i = 0; i < ag->npop; ++i){
        sorted_fitness[i].f = ag->fitness[i];
        sorted_fitness[i].i = i;
    }
    // Ordena o vetor de acordo com o valor de fitness
    sortVetPairDoubleInt(sorted_fitness, ag->npop);

    // Ranqueia os indivíduos pelo seu fitness
    double* ranked_fitness = (double*) malloc (sizeof(double) * ag->npop);
    for(int i = 0; i < ag->npop; ++i){
        ranked_fitness[sorted_fitness[i].i] = i + 1;
    }

    // Acumula os ranks para poder aplicar a roleta
    double* prefix_sum = accumulateVetDouble(ranked_fitness, ag->npop);
    int n_prefix_sum = ag->npop + 1;

    for(int i = 0; i < ag->npop; ++i){
        vparents[i] = rollTheRoulette(prefix_sum, n_prefix_sum);
    }

    // Libera os vetores da memória
    free(prefix_sum);
    free(sorted_fitness);
    free(ranked_fitness);

    return vparents;
}

int rollTheRoulette(const double *prefix_sum, const int n){
    double r = ((double) rand() / RAND_MAX) * prefix_sum[n-1];

    int low = 0, high = n-1;
    while(low < high){
        int mid = (low + high) / 2;

        if(r <= prefix_sum[mid]){
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return high == 0 ? 0 : high - 1;
}

void crossover(AG *ag, const int *parents){
    int i;
    for(i = 0; i < ag->npop - 1; i+= 2){
        double r = (double) rand() / RAND_MAX;

        int parent_1 = parents[i];
        int parent_2 = parents[i+1];

        // Se o cruzamento não deva ocorrer, apenas copia os genes dos pais
        if(r > ag->pcrossover){
            copyBackpack(ag->intermediate_backpacks[i], ag->backpacks[i]);
            copyBackpack(ag->intermediate_backpacks[i+1], ag->backpacks[i+1]);
            continue;
        }

        int crossover_point_1 = rand() % ag->backpacks[i]->nitems;
        int crossover_point_2 = randInRange(crossover_point_1, ag->backpacks[i]->nitems);
        int crossover_point_3 = randInRange(crossover_point_2, ag->backpacks[i]->nitems);

        for(int j = 0; j < ag->backpacks[i]->nitems; ++j){
            int item_p1 = ag->backpacks[parent_1]->items[j];
            int item_p2 = ag->backpacks[parent_2]->items[j];

            if((j < crossover_point_1) ||
               (j >= crossover_point_2 && j < crossover_point_3)){
                ag->intermediate_backpacks[i]->items[j] = item_p1;
                ag->intermediate_backpacks[i+1]->items[j] = item_p2;
            } else {
                ag->intermediate_backpacks[i]->items[j] = item_p2;
                ag->intermediate_backpacks[i+1]->items[j] = item_p1;
            }
        }
    }
    // Realiza o cruzamento para o último filho caso o tamanho da população
    // seja ímpar
    if(i < ag->npop){
        int parent_1 = parents[i];
        int parent_2 = parents[i-1];

        int crossover_point_1 = rand() % ag->backpacks[i]->nitems;
        int crossover_point_2 = randInRange(crossover_point_1, ag->backpacks[i]->nitems);
        int crossover_point_3 = randInRange(crossover_point_2, ag->backpacks[i]->nitems);

        for(int j = 0; j < ag->backpacks[i]->nitems; ++j){
            int item_p1 = ag->backpacks[parent_1]->items[j];
            int item_p2 = ag->backpacks[parent_2]->items[j];

            if((j < crossover_point_1) ||
               (j >= crossover_point_2 && j < crossover_point_3))
                ag->intermediate_backpacks[i]->items[j] = item_p1;
            else
                ag->intermediate_backpacks[i]->items[j] = item_p2;
        }
    }
}

void mutation(AG *ag){
    PairDoubleInt curr_local_max = maxDoubleVet(ag->fitness, ag->npop);

    if(curr_local_max.f > ag->local_max){
        ag->local_max = curr_local_max.f;
        ag->count_gen_local_max = 0;
        ag->extra_pmutation = 0.0;
    } else {
        ag->count_gen_local_max += 1;
        if(ag->count_gen_local_max >= 10){
            ag->extra_pmutation = 0.10;
        }
    }

    for(int i = 0; i < ag->npop; ++i){
        double r = (double) rand() / RAND_MAX;

        if(r <= (ag->pmutation + ag->extra_pmutation)){
            assignRandomItemsToBackpack(ag->intermediate_backpacks[i]);
        }
    }
}

void elitism(AG *ag){
    MaxHeap *heap = constroiMaxHeap(ag->fitness, ag->npop);

    for(int i = 0; i < ag->nelite; ++i){
        No backpack = extrairMaxHeap(heap);

        copyBackpack(ag->intermediate_backpacks[i], ag->backpacks[backpack.id]);
    }

    destroiMaxHeap(heap);
}

void copyPopulation(AG *ag){
    for(int idv = 0; idv < ag->npop; ++idv){
        copyBackpack(ag->backpacks[idv], ag->intermediate_backpacks[idv]);
    }
}

void freeAG(AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        freeBackpack(ag->backpacks[i]);
        freeBackpack(ag->intermediate_backpacks[i]);
    }
    free(ag->backpacks);
    free(ag->intermediate_backpacks);
    free(ag->fitness);
    free(ag);
}

void printPopulation(const AG *ag){
    for(int i = 0; i < ag->npop; ++i){
        printf("Mochila %d:\n", i);
        printf("\tItens: ");
        for(int j = 0; j < ag->backpacks[i]->nitems; ++j){
            printf("%d ", ag->backpacks[i]->items[j]);
        }
        printf("\n");
        printf("Fitness: %lf\n", ag->fitness[i]);
        printf("\n");
    }
}

void printResults(const AG *ag){
    PairDoubleInt r = maxDoubleVet(ag->fitness, ag->npop);

    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("                  Resumo dos Resultados                    \n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf(" Melhor fitness (maior utilidade): %lf                     \n", r.f);
    printf(" Itens: ");
    for(int i = 0; i < ag->backpacks[r.i]->nitems; ++i)
        printf("%d ", ag->backpacks[r.i]->items[i]);
    printf("\n");
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

void printGenIdfCSV(const AG *ag, FILE *fptr){
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

void printGenResultsCSV(const AG *ag, FILE *fptr, const int generation){
    int n_values = ag->npop + 1;
    char **values = (char**) malloc (sizeof(char*) * n_values);

    // Inserindo a geração na primeira posição do vetor
    int gen_len = getIntLen(generation);
    values[0] = (char*) malloc (gen_len * sizeof(char));
    if(values[0] == NULL){
        printf("Erro ao alocar memoria para o vetor de strings a serem escritas"
               " no arquivo .csv.\n");
        free(values);
        return;
    }
    char *generation_str = intToString(generation, gen_len);
    strcpy(values[0], generation_str);
    free(generation_str);

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
    const int n_contents = 7;
    char **contents = (char**) malloc (sizeof(char*) * n_contents);

    // Inserindo a coluna de probabilidade de cruzamento
    contents[0] = getDynamicAllocatedString("Probabilidade de Cruzamento");

    // Inserindo a coluna de quantidade de elites
    contents[1] = getDynamicAllocatedString("Elites");

    // Inserindo a coluna de probabilidade de mutação
    contents[2] = getDynamicAllocatedString("Probabilidade de Mutacao");

    // Inserindo a coluna de número de gerações
    contents[3] = getDynamicAllocatedString("Geracoes");

    // Inserindo a coluna de tamanho da população
    contents[4] = getDynamicAllocatedString("Tamanho da Populacao");

    // Inserindo a coluna de fitness do indivíduo de melhor aptidão
    contents[5] = getDynamicAllocatedString("Individuo de Melhor Aptidao");

    // Inserindo a coluna de aptidão média dos indivíduos
    contents[6] = getDynamicAllocatedString("Aptidao Media");

    // Escrevendo os identificadores das colunas no arquivo .csv
    writeLineCSV(fptr, contents, n_contents);

    // Liberando as strings da memória
    for(int i = 0; i < n_contents; ++i)
        free(contents[i]);
    free(contents);
}

void printFinalResultsCSV(const AG *ag, FILE *fptr){
    const int n_values = 7;
    char **values = (char**) malloc (sizeof(char*) * n_values);

    // Inserindo as taxas de cruzamento
    values[0] = doubleToString(ag->pcrossover, getDoubleLen(ag->pcrossover));

    // Inserindo a quantidade de elites
    values[1] = intToString(ag->nelite, getIntLen(ag->nelite));

    // Inserindo a probabilidade de mutação
    values[2] = doubleToString(ag->pmutation, getDoubleLen(ag->pmutation));

    // Inserindo o número de gerações
    values[3] = intToString(ag->ngen, getIntLen(ag->ngen));

    // Inserindo o tamanho da população
    values[4] = intToString(ag->npop, getIntLen(ag->npop));

    // Inserindo o fitness do indivíduo de melhor aptidão
    PairDoubleInt best_fit = maxDoubleVet(ag->fitness, ag->npop);
    int best_fit_len = getDoubleLen(best_fit.f);
    values[5] = doubleToString(best_fit.f, best_fit_len);

    // Inserindo a aptidão média dos indivíduos
    double average = 0.0;
    for(int i = 0; i < ag->npop; ++i)
        average += ag->fitness[i] >= 0.0 ? ag->fitness[i] : 0.0;
    average /= ag->npop;

    int average_len = getDoubleLen(average);
    values[6] = doubleToString(average, average_len);

    // Escreve os conteúdos no arquivo .csv
    writeLineCSV(fptr, values, n_values);

    // Liberando o vetor de strings da memória
    for(int i = 0; i < n_values; ++i)
        free(values[i]);
    free(values);

    return;
}
