#include "io.hpp"

#include <bits/stdc++.h>

using namespace std;

string Io::getFileName(const int &argc, char **argv, string &flag){
    string file_name;
    for(int i = 1; i < argc; i+= 2){
        string argument = argv[i];
        if(argument == flag){
            file_name = argv[i+1];
            return file_name;
        }
    }

    return "";
}

string Io::getFacOutputFileName(const int &argc, char **argv) {
    string flag = "-o";
    string fac_output_file_name = getFileName(argc, argv, flag);
    if(fac_output_file_name == "")
        fac_output_file_name = "results.csv";
    fac_output_file_name = "../data/" + fac_output_file_name;
    return fac_output_file_name;
}

string Io::getGenOutputFileName(const int &argc, char **argv) {
    string flag = "-o";
    string gen_output_file_name = getFileName(argc, argv, flag);
    if(gen_output_file_name == "")
        gen_output_file_name = "generations.csv";
    gen_output_file_name = "../data/" + gen_output_file_name;
    return gen_output_file_name;
}

int Io::getSolutionId(const int &argc, char **argv){
    for(int i = 1; i < argc; i+=2){
        if(not strcmp(argv[i], "-s")){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

string Io::getInputFileName(const int &argc, char **argv){
    string flag = "-i";
    string input_file_name = getFileName(argc, argv, flag);

    if(input_file_name == ""){
        input_file_name = "sgb128_dist.txt";
    }

    input_file_name = "../inputs/" + input_file_name;
    return input_file_name;
}
