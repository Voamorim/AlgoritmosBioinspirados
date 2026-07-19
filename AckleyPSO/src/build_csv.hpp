#ifndef BUILD_CSV_HPP
#define BUILD_CSV_HPP

#include "pso.hpp"

#include <bits/stdc++.h>

using namespace std;

class BuildCSV{
    public:
        void printGenIdfCSV(const Pso *pso, ofstream &gen_file);
        void printFacIdfCSV(ofstream &fac_file);

        void printFacResultsCSV(const Pso *pso, ofstream &fac_file);

        void printParticlesDimensionsCSV(const Pso *pso, ofstream &pos_file);
        void printParticlesPosCSV(const Pso *pso, const int geracao, ofstream &pos_file);
};

#endif