#include "min_heap.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Função para manter a propriedade da min_heap para a subárvore enraizada no
// índice i
// Assume que as subárvores à esquerda e à direita já são min_heaps
void min_heapify(MinHeap *heap, int i){
    int menor = i; // Assume que o nó atual é o menor

    // Obtém os índices dos filhos à esquerda e à direita
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    // Verifica se o filho à esquerda é menor que o nó atual
    if(esq < heap->tam && heap->array[esq].valor < heap->array[menor].valor){
        menor = esq;
    }

    // Verifica se o filho à direita é menor que o menor até agora
    if(dir < heap->tam && heap->array[dir].valor < heap->array[menor].valor){
        menor = dir;
    }

    // Se o menor não é o nó atual, significa que um dos filhos é menor
    if(menor != i){
        // Troca o nó atual com o menor filho
        swapNo(&heap->array[i], &heap->array[menor]);


        // Chama recursivamente min_heapify na subárvore afetada
        min_heapify(heap, menor);
    }
}

// Cria a estrutura min_heap com a capacidade máxima definida
MinHeap* criaMinHeap(int max_tam){
    MinHeap* heap = (MinHeap*) malloc (sizeof(MinHeap));

    if(heap == NULL){
        perror("Erro ao alocar memória para a estrutura min_heap.");
        return NULL;
    }

    heap->max_tam = max_tam;
    heap->tam = 0; // Inicialmente a heap está vazia

    heap->array = (No*) calloc (max_tam, sizeof(No));

    if(heap->array == NULL){
        perror("Erro ao alocar memória para o vetor da estrutura min_heap.");
        free(heap);
        return NULL;
    }

    return heap;
}

// Verifica se a heap está cheia
int heapCheia(const MinHeap *heap){
    return heap->tam == heap->max_tam;
}

// Verifica se a heap está vazia
int heapVazia(const MinHeap *heap){
    return heap->tam == 0;
}

// Insere uma nova chave na min_heap
void insereMinHeap(MinHeap *heap, double valor, int id){
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


    // Mantém a propriedade da min_heap movendo o novo elemento para cima da
    // árvore (caso necessário)

    // "Enquanto o elemento atual não for a raíz e ser maior que o seu pai..."
    while(i != 0 && heap->array[(i - 1)/2].valor > heap->array[i].valor){
        swapNo(&heap->array[i], &heap->array[(i-1)/2]);
        i = (i-1)/2; // Move o índice para o pai
    }
}

No extrairMinHeap(MinHeap *heap) {
    if(heapVazia(heap)){
        perror("Nao foi possivel extrair elementos. A heap esta vazia.");
        No erro;
        erro.valor = INT_MAX; erro.id = -1;
        return erro;
    }

    // O nó de valor mínimo sempre está na raíz
    No raiz = heap->array[0];

    // Se houver mais de um elemento na heap
    if(heap->tam > 1){

        // Move o último elemento para a raíz
        heap->array[0] = heap->array[heap->tam-1];

        heap->tam -= 1; // Decrementa o tamanho da heap

        // Chama min_heapfy na raíz para restaurar a propriedade da min_heap
        min_heapify(heap, 0);
    } else {
        heap->tam -= 1;
    }

    return raiz;
}

// Libera a memória alocada para a estrutura min_heap
void destroiMinHeap(MinHeap *heap){
    if(heap){
        free(heap->array);
        free(heap);
    }
}

// Constrói uma min_heap a partir de um vetor existente
MinHeap* constroiMinHeap(double *v, int tam){
    MinHeap *heap = criaMinHeap(tam);

    if(heap == NULL) return NULL;

    // Copia os elementos do array para o array da heap
    for(int i = 0; i < tam; ++i){
        heap->array[i].valor = v[i];
        heap->array[i].id = i;
    }
    heap->tam = tam;

    // Converte o array em uma min_heap chamando min_heapify em todos os nós
    // não folha
    for(int i = (heap->tam / 2) - 1; i >= 0; --i){
        min_heapify(heap, i);
    }

    return heap;
}

void swapNo(struct No *a, struct No *b){
    struct No aux = *a;
    *a = *b;
    *b = aux;
}
