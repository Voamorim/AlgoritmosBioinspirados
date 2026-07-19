#include "build_csv.hpp"

void BuildCSV::printGenIdfCSV(const Pso *pso, ofstream &gen_file){
    gen_file << "Geracao";
    for(int i = 1; i <= pso->getNpop(); ++i)
        gen_file << ",Individuo " << i;
    gen_file << '\n';
}

void BuildCSV::printFacIdfCSV(ofstream &fac_file){
    fac_file << "Geracoes,";
    fac_file << "Tamanho da Populacao,";
    fac_file << "Dimensoes,";
    fac_file << "Fator Cognitivo,";
    fac_file << "Fator Social,";
    fac_file << "Fator de Diversificacao,";
    fac_file << "Melhor Aptidao,";
    fac_file << "Geracao Encontrada,";
    fac_file << "Aptidao Media" << '\n';
}

void BuildCSV::printFacResultsCSV(const Pso *pso, ofstream &fac_file){
    cout << fixed << setprecision(2);

    fac_file << pso->getNgen() << ',';
    fac_file << pso->getNpop() << ','; 
    fac_file << pso->getNdimensions() << ',';
    fac_file << pso->getC1() << ',';
    fac_file << pso->getC2() << ',';
    fac_file << pso->getW() << ',';
    fac_file << pso->getGbestFitness() << ',';
    fac_file << pso->found_min << ',';
    fac_file << pso->getAverageFitness() << '\n';
}

void BuildCSV::printParticlesDimensionsCSV(const Pso *pso, ofstream &pos_file){
    pos_file << "Geracao,";
    pos_file << "ID_particula,";
    const string dimension = "Dimensao";

    for(int i = 1; i <= pso->getNdimensions(); ++i){
        pos_file << dimension << " " << i << ",";
    }

    pos_file << "Fitness" << "\n";
}

void BuildCSV::printParticlesPosCSV(const Pso *pso, const int geracao, ofstream &pos_file){
    for(int i = 0; i < pso->getNpop(); ++i){
        pos_file << geracao << ',' << i;
        for(int d = 0; d < pso->getNdimensions(); ++d){
            pos_file << ',' << pso->particles[i]->getPos()[d];
        }
        pos_file << ',' << pso->particles[i]->getFitness() << "\n";
    }
}