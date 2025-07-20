#ifndef ALGORITMO_GENETICO
#define ALGORITMO_GENETICO

typedef struct AG {
    int npop; // Tamanho da população
    int nger; // Quantidade de gerações
    int nelite; // Quantidade de elementos para o elitismo
    int ngenes; // Quantidade de genes por membro da população
    int ncromossomos; // Quantidade de cromossomos por indivíduos

    float pmutacao; // Taxa de mutação
    int pcruzamento; // Probabilidade de cruzamento

    int** pop;
    int** pop_intermediaria;

    float* fitness;
} AG;

AG* criaAG(int npop, int nger, int nelite, int ngenes, int ncromossomos,
           float pmutacao, float pcruzamento);
void criaPopulacaoInicial(AG *ag);
void avaliaPopulacao(AG *ag);
float ackley(float x[], int n);
int* torneio(AG *ag);
void cruzamento(AG *ag, int* vpais);
void mutacao(AG *ag);
void elitismo(AG *ag);

void destroiAG(AG *ag);
void copiaPopulacao(AG *ag);

void printPopulacao(AG *ag);
void printResults(AG *ag);

#endif
