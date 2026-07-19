#ifndef MAX_HEAP
#define MAX_HEAP

typedef struct No {
    double valor;
    int id;
} No;

typedef struct MaxHeap {
    No *array;
    int tam;
    int max_tam;
} MaxHeap;

void max_heapify(MaxHeap *heap, int i);
MaxHeap* criaMaxHeap(int max_tam);
int heapCheia(const MaxHeap *heap);
int heapVazia(const MaxHeap *heap);
void insereMaxHeap(MaxHeap *heap, double valor, int id);
No extrairMaxHeap(MaxHeap *heap);
void destroiMaxHeap(MaxHeap *heap);
MaxHeap* constroiMaxHeap(double *v, int tam);

void swapNo(struct No *a, struct No *b);

#endif
