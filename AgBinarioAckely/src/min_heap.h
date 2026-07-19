#ifndef MIN_HEAP
#define MIN_HEAP

typedef struct No {
    float valor;
    int id;
} No;

typedef struct MinHeap {
    No *array;
    int tam;
    int max_tam;
} MinHeap;

void min_heapify(MinHeap *heap, int i);
MinHeap* criaMinHeap(int max_tam);
int heapCheia(const MinHeap *heap);
int heapVazia(const MinHeap *heap);
void insereMinHeap(MinHeap *heap, float valor, int id);
No extrairMinHeap(MinHeap *heap);
void destroiMinHeap(MinHeap *heap);
MinHeap* constroiMinHeap(float *v, int tam);

void swapNo(struct No *a, struct No *b);

#endif
