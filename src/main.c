/**
 * Atividade 1 - Algoritmo Genético
 *
 * Aluno: Vítor Oliveira Amorim
 * Professora: Carolina Ribeiro Xavier
 * Disciplina: Algoritmos Bioinspirados
*/

#include "ag.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

int main(){
    const int npop = 20;
    const int nger = 150;
    const int nelite = 2;
    const int ngenes = 31;
    const int ncromossomos = 2;
    const float pcruzamento = 0.95f;
    const float pmutacao = 0.05f;

    AG *ag = criaAG(npop, nger, nelite, ngenes, ncromossomos,
                    pmutacao, pcruzamento);

    if(ag == NULL){
        printf("Erro ao tentar criar o Algoritmo Genetico.\n");
        return 0;
    }

    criaPopulacaoInicial(ag);

    for(int g = 0; g < ag->nger; ++g){
        avaliaPopulacao(ag);
        int *vpais = torneio(ag);
        cruzamento(ag, vpais);
        mutacao(ag);
        elitismo(ag);

        copiaPopulacao(ag);

        free(vpais);
    }

    printResults(ag);

    destroiAG(ag);
    return 0;
}
