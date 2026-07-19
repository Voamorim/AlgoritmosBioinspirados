#include "build_csv.hpp"

using namespace std;

void BuildCSV::printGenIdfCSV(const AntSystem *ant_system, ofstream &gen_file){
    gen_file << "Geracao";
    for(int i = 1; i <= ant_system->npop; ++i){
        gen_file << ",Individuo " << i;
    }
    gen_file << '\n';
}

void BuildCSV::printFacIdfCSV(ofstream &fac_file){
    fac_file << "Geracoes,";
    fac_file << "Tamanho da Populacao,";
    fac_file << "Q,";
    fac_file << "Alfa,";
    fac_file << "Beta,";
    fac_file << "p,";
    fac_file << "Melhor Aptidao,";
    fac_file << "Aptidao Media" << '\n';
}

void BuildCSV::printFacResultsCSV(AntSystem *ant_system, ofstream &fac_file){
    fac_file << ant_system->num_iterations << ',';
    fac_file << ant_system->npop << ',';
    fac_file << ant_system->Q << ',';
    fac_file << fixed << setprecision(2) << ant_system->alfa << ',';
    fac_file << fixed << setprecision(2) << ant_system->beta << ',';
    fac_file << fixed << setprecision(2) << ant_system->p << ',';
    fac_file << fixed << setprecision(2) << ant_system->min_cycle << ',';
    fac_file << fixed << setprecision(2) << ant_system->sum_costs/(ant_system->npop * ant_system->num_iterations);
    fac_file << '\n';
}
