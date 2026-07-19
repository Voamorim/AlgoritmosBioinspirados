#ifndef BUILD_CSV_HPP
#define BUILD_CSV_HPP

#include "pcv.hpp"

#include <bits/stdc++.h>

using namespace std;

class BuildCSV{
    public:
        void printGenIdfCSV(Pcv *pcv, ofstream &gen_file);
        void printFacIdfCSV(ofstream &fac_file);

        void printGenResultsCSV(Pcv *pcv, ofstream &gen_file, const int gen);
        void printFacResultsCSV(Pcv *pcv, ofstream &fac_file);
};

#endif
