#ifndef SOLVE_HPP
#define SOLVE_HPP

#include "pso.hpp"
#include "build_csv.hpp"

#include <bits/stdc++.h>

using namespace std;

class Solve{
    private:
        void solveFactorialTest(const int npop, const int ngen, 
                                const int ndimensions, const long double c1,
                                const long double c2, const long double w,
                                const int topology_id, ofstream &output_file);
        void solveGenerationsTest(const int npop, const int ngen, 
                                  const int ndimensions, const long double c1,
                                  const long double c2, const long double w,
                                  const int topology_id, ofstream &output_file);
    public:
        void solve(const int topology_id);
        void factorialTest(const int topology_id, ofstream &output_file);
        void generationsTest(const int topology_id, ofstream &output_file);
};

#endif