#ifndef IO_HPP
#define IO_HPP

#include <bits/stdc++.h>

using namespace std;

class Io{
    private:
        string getFileName(const int &argc, char **argv, string &flag);
    public:
        string getInputFileName(const int &argc, char **argv);
        string getFacOutputFileName(const int &argc, char **argv);
        string getGenOutputFileName(const int &argc, char **argv);
        int getSolutionId(const int &argc, char **argv);
};

#endif
