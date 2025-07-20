#ifndef SOLVE
#define SOLVE

void solve(void);

void factorialTest(char *results_file_name);
void generationsTest(char *generations_file_name);

void solveFactorialTest(const int npop, const int nger, const int nelite,
                        const int ncromossomos, const float pmutacao,
                        const float pcruzamento, const float alfa,
                        const float beta, FILE *results);
void solveGenerationsTest(const int npop, const int nger, const int nelite,
                          const int ncromossomos, const float pmutacao,
                          const float pcruzamento, const float alfa,
                          const float beta, FILE *generation_results);

#endif
