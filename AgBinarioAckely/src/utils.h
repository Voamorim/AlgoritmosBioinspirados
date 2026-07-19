#ifndef UTILS
#define UTILS

typedef struct PairFloatInt{
    float f;
    int i;
} PairFloatInt;

int binToUnsignedInt(int* bin, int n);
float binToFloat(int *bin, int n, float xmin, float xmax);
void swapInt(int *a, int *b);
float minFloat(const float a, const float b);
PairFloatInt minFloatVet(float* v, const int n);

#endif
