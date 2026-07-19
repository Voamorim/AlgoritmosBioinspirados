#include <bits/stdc++.h>

#include "build_csv.hpp"
#include "pcv.hpp"

using namespace std;

void BuildCSV::printGenIdfCSV(Pcv *pcv, ofstream &gen_file){
    gen_file << "Geracao";
    for(int i = 1; i <= pcv->npop; ++i)
        gen_file << ",Individuo " << i;
    gen_file << "\n";
}

void BuildCSV::printFacIdfCSV(ofstream &fac_file){
    fac_file << "Probabilidade de Cruzamento,";
    fac_file << "Elites,";
    fac_file << "Probabilidade de Mutacao,";
    fac_file << "Geracoes,";
    fac_file << "Tamanho da Populacao,";
    fac_file << "Melhor Aptidao,";
    fac_file << "Aptidao Media,";
    fac_file << "Desvio Padrao" << '\n';
}

void BuildCSV::printGenResultsCSV(Pcv *pcv, ofstream &gen_file, const int gen){
    gen_file << gen;
    for(int i = 0; i < pcv->npop; ++i){
        gen_file << ',' << pcv->fitness[i];
    }
    gen_file << '\n';
}

void BuildCSV::printFacResultsCSV(Pcv *pcv, ofstream &fac_file){
    fac_file << fixed << setprecision(2) << pcv->pcrossover << ',';
    fac_file << pcv->nelite << ',';
    fac_file << fixed << setprecision(2) << pcv->pmutation << ',';
    fac_file << pcv->ngen << ',';
    fac_file << pcv->npop << ',';
    fac_file << *min_element(pcv->fitness.begin(), pcv->fitness.end()) << ',';

    double average_fit = accumulate(pcv->fitness.begin(), pcv->fitness.end(), 0.0);
    average_fit /= pcv->npop;

    double dp = 0.0;
    for(int fit : pcv->fitness){
        dp += pow(fit - average_fit, 2);
    }
    dp = sqrt(dp/pcv->npop);

    fac_file << fixed << setprecision(6) << average_fit << ',';
    fac_file << fixed << setprecision(6) << dp << '\n';
}
