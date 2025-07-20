/**
 * Atividade 3 - Mochila Binária
 *
 * Aluno: Vítor Oliveira Amorim
 * Professora: Carolina Ribeiro Xavier
 * Disciplina: Algoritmos Bioinspirados
*/

#include "ag.h"
#include "io.h"
#include "solve.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Arquivos para escrever os resultados
#define FACTORIAL_TEST_RESULTS_FILE "../data/results.csv"
#define GENERATIONS_RESULTS_FILE "../data/generations.csv"

int main(int argc, char **argv){
    srand(time(NULL));

    // Seleciona qual das soluções será executada
    // Por padrão, executa a solução simples
    int solution = getSolutionId(argc, argv);

    // Obtém os arquivos contendo as informações do problema
    FILE *items_utility_file = getUtilityFile(argc, argv);
    FILE *items_weights_file = getWeightsFile(argc, argv);
    FILE *backpack_volume_file = getBackpackFile(argc, argv);

    // Seleciona a solução
    switch(solution){
        case 1:
            solve(items_weights_file, items_utility_file, backpack_volume_file);
            break;
        case 2:
            generationsTest(GENERATIONS_RESULTS_FILE, items_weights_file,
                            items_utility_file, backpack_volume_file);
            break;
        case 3:
            factorialTest(FACTORIAL_TEST_RESULTS_FILE, items_weights_file,
                          items_utility_file, backpack_volume_file);
            break;
    }

    return 0;
}
