#include "io.hpp"

string Io::getFileName(const int &argc, char **argv, const string &flag){
    string file_name;
    for(int i = 1; i < argc; i += 2){
        string argument = argv[i];
        if(argument == flag){
            file_name = argv[i+1];
            return file_name;
        }
    }

    return "";
}

string Io::getFacOutputFileName(const int &argc, char **argv){
    string flag = "-o";
    string fac_output_file_name = getFileName(argc, argv, flag);
    if(fac_output_file_name == "")
        fac_output_file_name = "results.csv";
    fac_output_file_name = "../data/tables/" + fac_output_file_name;
    return fac_output_file_name;
}

string Io::getGenOutputFileName(const int &argc, char **argv){
    string flag = "-o";
    string gen_output_file_name = getFileName(argc, argv, flag);
    if(gen_output_file_name == "")
        gen_output_file_name = "generations.csv";
    gen_output_file_name = "../data/tables/" + gen_output_file_name;
    return gen_output_file_name;
}

int Io::getId(const int &argc, char **argv, const string &flag){
    for(int i = 1; i < argc; i+=2){
        if(not strcmp(argv[i], flag.c_str())){
            return atoi(argv[i+1]);
        }
    }
    return 1;
}

int Io::getSolutionId(const int &argc, char **argv){
    return getId(argc, argv, "-s");
}

int Io::getTopologyId(const int &argc, char **argv){
    return getId(argc, argv, "t");
}