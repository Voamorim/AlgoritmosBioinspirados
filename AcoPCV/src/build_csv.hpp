#ifndef BUILD_CSV_HPP
#define BUILD_CSV_HPP

#include "ant_system.hpp"

#include <bits/stdc++.h>

using namespace std;

class BuildCSV{
    public:
        void printGenIdfCSV(const AntSystem *ant_system, ofstream &gen_file);
        void printFacIdfCSV(ofstream &fac_file);

        void printFacResultsCSV(AntSystem *ant_system, ofstream &fac_file);
};

#endif
