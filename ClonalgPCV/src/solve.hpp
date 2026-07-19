#ifndef SOLVE_HPP
#define SOLVE_HPP

#include "clonalg.hpp"
#include "graph.hpp"
#include "build_csv.hpp"

#include <bits/stdc++.h>

using namespace std;

typedef unsigned uint;

class Solve{
    private:
        void solveFactorialTest(const Graph *graph, const uint npop, 
                                const uint nclones, const uint ngen, 
                                const uint d, const double beta, 
                                const double rho, ofstream &output_file);
        void solveGenerationsTest(const Graph *graph, const uint npop,
                                  const uint nclones, const uint ngen,
                                  const uint d, const double beta,
                                  const double rho, ofstream &output_file);
    public:
        void solve(ifstream &input_file);
        void factorialTest(ifstream &input_file, ofstream &output_file);
        void generationsTest(ifstream &input_file, ofstream &output_file);
};

#endif