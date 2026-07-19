#include "solve.hpp"
#include "pso.hpp"
#include "build_csv.hpp"

void Solve::solve(const int topology_id){
    const int npop = 225;
    const int ngen = 200;
    const int ndimensions = 2;

    const long double c1 = 0.25;
    const long double c2 = 0.75;
    const long double w = 0.7;

    Pso *pso = new Pso(npop, ngen, ndimensions, c1, c2, w);

    pso->createInitialPopulation();
    for(int generation = 0; generation < pso->getNgen(); ++generation){
        pso->current_generation = generation; 
        pso->updateParticles(topology_id);
    }
    pso->printBestPos();
    delete pso;
}

void Solve::factorialTest(const int topology_id, ofstream &output_file){
    const int npop = 225;
    const int ngen = 200; 
    array<int, 3> ndimensions = {2, 3, 4};

    array<long double, 5> c1 = {0.0, 0.25, 0.5, 0.75, 1.0};
    array<long double, 5> c2 = {1.0, 0.75, 0.5, 0.25, 0.0};
    array<long double, 4> w = {0.2, 0.5, 0.7, 0.9};

    BuildCSV *csv_builder = new BuildCSV();
    csv_builder->printFacIdfCSV(output_file);
    delete csv_builder;

    for(int indimensions = 0; indimensions < (int) ndimensions.size(); 
                                                                ++indimensions){
        for(int ic = 0; ic < (int) c1.size(); ++ic){
            for (int iw = 0; iw < (int)w.size(); ++iw){
                solveFactorialTest(npop, ngen, ndimensions[indimensions],
                                   c1[ic], c2[ic], w[iw], topology_id,
                                   output_file);
            }
        }
    }

    output_file.close();
}

void Solve::generationsTest(const int topology_id, ofstream &output_file){
    const int npop = 225;
    const int ngen = 200;
    const int ndimensions = 2;

    const long double c1 = 0.5;
    const long double c2 = 0.5;
    const long double w = 0.7;

    solveGenerationsTest(npop, ngen, ndimensions, c1, c2, w, topology_id,
                         output_file);

    output_file.close();
}

void Solve::solveFactorialTest(const int npop, const int ngen, 
                               const int ndimensions, const long double c1, 
                               const long double c2, const long double w, 
                               const int topology_id, ofstream &output_file){
    Pso *pso = new Pso(npop, ngen, ndimensions, c1, c2, w);

    pso->createInitialPopulation();
    for(int generation = 0; generation < pso->getNgen(); ++generation){
        pso->current_generation = generation;
        pso->updateParticles(topology_id);
    }

    BuildCSV *csv_builder = new BuildCSV();
    csv_builder->printFacResultsCSV(pso, output_file);
    delete csv_builder;

    delete pso;
}

void Solve::solveGenerationsTest(const int npop, const int ngen, 
                                 const int ndimensions, const long double c1, 
                                 const long double c2, const long double w, 
                                 const int topology_id, ofstream &output_file){
    Pso *pso = new Pso(npop, ngen, ndimensions, c1, c2, w);

    BuildCSV *csv_builder = new BuildCSV();
    csv_builder->printGenIdfCSV(pso, output_file);

    ofstream pos_file("../data/tables/pos_file.csv");

    if(not pos_file.is_open()){
        cerr << "Erro ao abrir o arquivo para armazenar a posicao das particulas por"
             << "geracao.\n";
        exit(1);
    }

    csv_builder->printParticlesDimensionsCSV(pso, pos_file);

    pso->createInitialPopulation();
    for(int generation = 0; generation < pso->getNgen(); ++generation){
        pso->current_generation = generation;

        output_file << generation << ',';
        pso->updateParticles(topology_id);

        for(int p = 0; p < pso->getNpop() - 1; ++p){
            output_file << pso->particles[p]->getBestFitness() << ',';
        }
        output_file << pso->particles[pso->getNpop() - 1]->getFitness() << '\n';

        csv_builder->printParticlesPosCSV(pso, generation, pos_file);

    }

    pos_file.close();

    delete csv_builder;
    delete pso;
}