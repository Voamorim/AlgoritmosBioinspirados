#include "max_heap.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Função para manter a propriedade da max_heap para a subárvore enraizada no
// índice i
// Assume que as subárvores à esquerda e à direita já são max_heaps
void max_heapify(MaxHeap *heap, int i){
    int maior = i; // Assume que o nó atual é o maior

    // Obtém os índices dos filhos à esquerda e à direita
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    // Verifica se o filho à esquerda é maior que o nó atual
    if(esq < heap->tam && heap->array[esq].valor > heap->array[maior].valor){
        maior = esq;
    }

    // Verifica se o filho à direita é maior que o maior até agora
    if(dir < heap->tam && heap->array[dir].valor > heap->array[maior].valor){
        maior = dir;
    }

    // Se o maior não é o nó atual, significa que um dos filhos é maior
    if(maior != i){
        // Troca o nó atual com o maior filho
        swapNo(&heap->array[i], &heap->array[maior]);


        // Chama recursivamente max_heapify na subárvore afetada
        max_heapify(heap, maior);
    }
}

// Cria a estrutura max_heap com a capacidade máxima definida
MaxHeap* criaMaxHeap(int max_tam){
    MaxHeap* heap = (MaxHeap*) malloc (sizeof(MaxHeap));

    if(heap == NULL){
        perror("Erro ao alocar memória para a estrutura max_heap.");
        return NULL;
    }

    heap->max_tam = max_tam;
    heap->tam = 0; // Inicialmente a heap está vazia

    heap->array = (No*) calloc (max_tam, sizeof(No));

    if(heap->array == NULL){
        perror("Erro ao alocar memória para o vetor da estrutura max_heap.");
        free(heap);
        return NULL;
    }

    return heap;
}

// Verifica se a heap está cheia
int heapCheia(const MaxHeap *heap){
    return heap->tam == heap->max_tam;
}

// Verifica se a heap está vazia
int heapVazia(const MaxHeap *heap){
    return heap->tam == 0;
}

// Insere uma nova chave na max_heap
void insereMaxHeap(MaxHeap *heap, double valor, int id){
    // Verifica se a estrutura está cheia
    if(heap == NULL || heapCheia(heap)){
        printf("Nao foi possivel inserir o elemento %d. A heap esta cheia "
               "ou é inválida.", id);
        return;
    }

    No no;
    no.valor = valor;
    no.id = id;

    int i = heap->tam; // Obtém o índice da última chave inserida
    heap->array[i] = no;
    heap->tam += 1;


    // Mantém a propriedade da max_heap movendo o novo elemento para cima da
    // árvore (caso necessário)

    // "Enquanto o elemento atual não for a raíz e ser menor que o seu pai..."
    while(i != 0 && heap->array[(i - 1)/2].valor < heap->array[i].valor){
        swapNo(&heap->array[i], &heap->array[(i-1)/2]);
        i = (i-1)/2; // Move o índice para o pai
    }
}

No extrairMaxHeap(MaxHeap *heap) {
    if(heapVazia(heap)){
        perror("Nao foi possivel extrair elementos. A heap esta vazia.");
        No erro;
        erro.valor = INT_MAX; erro.id = -1;
        return erro;
    }

    // O nó de valor máximo sempre está na raíz
    No raiz = heap->array[0];

    // Se houver mais de um elemento na heap
    if(heap->tam > 1){

        // Move o último elemento para a raíz
        heap->array[0] = heap->array[heap->tam-1];

        heap->tam -= 1; // Decrementa o tamanho da heap

        // Chama max_heapfy na raíz para restaurar a propriedade da max_heap
        max_heapify(heap, 0);
    } else {
        heap->tam -= 1;
    }

    return raiz;
}

// Libera a memória alocada para a estrutura max_heap
void destroiMaxHeap(MaxHeap *heap){
    if(heap){
        free(heap->array);
        free(heap);
    }
}

// Constrói uma max_heap a partir de um vetor existente
MaxHeap* constroiMaxHeap(double *v, int tam){
    MaxHeap *heap = criaMaxHeap(tam);

    if(heap == NULL) return NULL;

    // Copia os elementos do array para o array da heap
    for(int i = 0; i < tam; ++i){
        heap->array[i].valor = v[i];
        heap->array[i].id = i;
    }
    heap->tam = tam;

    // Converte o array em uma max_heap chamando max_heapify em todos os nós
    // não folha
    for(int i = (heap->tam / 2) - 1; i >= 0; --i){
        max_heapify(heap, i);
    }

    return heap;
}

void swapNo(struct No *a, struct No *b){
    struct No aux = *a;
    *a = *b;
    *b = aux;
}
