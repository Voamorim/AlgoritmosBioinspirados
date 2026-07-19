#include "build_csv.hpp"

using namespace std;

void BuildCSV::printGenIdfCSV(const Clonalg *clonalg, ofstream &gen_file){
    gen_file << "Geracao";
    for(uint i = 1; i <= clonalg->getNpop(); ++i){
        gen_file << ",Individuo " << i;
    }
    gen_file << '\n';
}

void BuildCSV::printFacIdfCSV(ofstream &fac_file){
    fac_file << "Geracoes,";
    fac_file << "Tamanho da Populacao,";
    fac_file << "Numero de clones,";
    fac_file << "d,";
    fac_file << "Beta,";
    fac_file << "Rho,";
    fac_file << "Melhor Aptidao,";
    fac_file << "Aptidao Media" << '\n';
}

void BuildCSV::printFacResultsCSV(Clonalg *clonalg, ofstream &fac_file){
    fac_file << clonalg->getNgen() << ',';
    fac_file << clonalg->getNpop() << ',';
    fac_file << clonalg->getNclones() << ',';
    fac_file << clonalg->getD() << ',';
    fac_file << fixed << setprecision(2) << clonalg->getBeta() << ',';
    fac_file << fixed << setprecision(2) << clonalg->getRho() << ',';
    fac_file << fixed << setprecision(2) << clonalg->getMinIndividualCost() << ',';
    fac_file << fixed << setprecision(2) << clonalg->getPopulationAverageCost();
    fac_file << '\n';
}

void BuildCSV::printGenResultsCSV(vector<pair<vector<uint>, double>>& population,
                                  uint generation, ofstream &gen_file){
    gen_file << generation; 
    for(auto& [cycle, cost] : population){
        gen_file << ',' << cost;
    }
    gen_file << '\n';
}