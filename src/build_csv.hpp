#ifndef BUILD_CSV_HPP
#define BUILD_CSV_HPP

#include "clonalg.hpp"

#include <bits/stdc++.h>

using namespace std;

typedef unsigned uint;

class BuildCSV{
    public:
        void printGenIdfCSV(const Clonalg *clonalg, ofstream &gen_file);
        void printFacIdfCSV(ofstream &fac_file);

        void printFacResultsCSV(Clonalg *clonalg, ofstream &fac_file);
        void printGenResultsCSV(vector<pair<vector<uint>, double>>& population,
                                uint generation, ofstream &gen_file);
};

#endif