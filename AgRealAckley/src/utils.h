#ifndef UTILS
#define UTILS

typedef struct PairDoubleInt{
    double f;
    int i;
} PairDoubleInt;

int isEven(const int a);
void swapInt(int *a, int *b);

// Funções para max() e min() entre dois valores
double minDouble(const double a, const double b);
double maxDouble(const double a, const double b);

PairDoubleInt minDoubleVet(double* v, const int n);
double sumVetDouble(const double v[], const int n);
double* accumulateVetDouble(const double v[], const int n);

double randInRange(const double low, const double high);

int getIntLen(const int n);
int getDoubleLen(const double n);
char* intToString(const int n, const int len);
char* doubleToString(const double n, const int len);
double getVetAverage(const double *v, const int n);
char* getDynamicAllocatedString(const char *str);

#endif
