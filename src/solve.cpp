#include "solve.hpp"

#define dbg(x) cout << #x << " == " << x << endl

void Solve::solve(ifstream &input_file){
    const uint npop = 200;
    const uint nclones = 100;
    const uint ngen = 100;
    const uint d = 5;
    const double beta = 2.0;
    const double rho = 1.0;

    // Cria e lê o grafo
    Graph *graph = new Graph();
    graph->readGraph(input_file);
    input_file.close();

    Clonalg *clonalg = new Clonalg(graph, npop, nclones, ngen, d, beta,
                                   rho);
   
    clonalg->createInitialPopulation();                                   

    for(uint generation = 1; generation <= clonalg->getNgen(); ++generation){
        clonalg->evaluatePopulation();
        auto clones = clonalg->createClones();
        clonalg->hypermutation(clones);
        clonalg->updateMemory(clones);
        clonalg->applyDiversity();
        clonalg->updatePopulation();
    }

    clonalg->printResults();

    delete graph;
    delete clonalg;
}

void Solve::generationsTest(ifstream &input_file, ofstream &output_file){
    const uint npop = 200;
    const uint nclones = 100;
    const uint ngen = 100;
    const uint d = 5;
    const double beta = 2.0;
    const double rho = 1.0;

    Graph *graph = new Graph();
    graph->readGraph(input_file);
    input_file.close();

    solveGenerationsTest(graph, npop, nclones, ngen, d, beta, rho, output_file);

    output_file.close();

    delete graph;
}

void Solve::factorialTest(ifstream &input_file, ofstream &output_file){
    const uint npop = 200; 
    array<uint, 3> nclones = {100, 150, 200};
    const uint ngen = 100;
    array<uint, 3> d = {2, 5, 10};
    array<double, 4> beta = {1.0, 2.0, 5.0, 10.0};
    array<double, 4> rho = {0.5, 1.0, 2.5, 5.0};

    Graph *graph = new Graph();
    graph->readGraph(input_file);
    input_file.close();

    BuildCSV *csv_builder = new BuildCSV();
    csv_builder->printFacIdfCSV(output_file);
    delete csv_builder;

    for(uint inclones = 0; inclones < nclones.size(); ++inclones){
        for(uint id = 0; id < d.size(); ++id){
            for(uint ibeta = 0; ibeta < beta.size(); ++ibeta){
                for(uint irho = 0; irho < rho.size(); ++irho){
                    solveFactorialTest(graph, npop, nclones[inclones],
                                       ngen, d[id], beta[ibeta], rho[irho],
                                       output_file);
                }
            }
        }
    }

    delete graph;
}

void Solve::solveGenerationsTest(const Graph *graph, const uint npop, 
                                 const uint nclones, const uint ngen, 
                                 const uint d, const double beta,
                                 const double rho, ofstream &output_file){
    Clonalg *clonalg = new Clonalg(graph, npop, nclones, ngen, d, beta,
                                   rho);

    BuildCSV *csv_builder = new BuildCSV();
    csv_builder->printGenIdfCSV(clonalg, output_file);

    clonalg->createInitialPopulation();

    for(uint generation = 1; generation <= clonalg->getNgen(); ++generation){
        clonalg->evaluatePopulation();
        auto clones = clonalg->createClones();
        clonalg->hypermutation(clones);
        clonalg->updateMemory(clones);
        clonalg->applyDiversity();
        clonalg->updatePopulation();

        auto population = clonalg->getPopulation();
        csv_builder->printGenResultsCSV(population, generation, output_file);
    }

    delete csv_builder;
    delete clonalg;
}

void Solve::solveFactorialTest(const Graph *graph, const uint npop, 
                               const uint nclones, const uint ngen, 
                               const uint d, const double beta,
                               const double rho, ofstream &output_file){
    Clonalg *clonalg = new Clonalg(graph, npop, nclones, ngen, d, beta,
                                   rho);
    
    clonalg->createInitialPopulation();

    for(uint generation = 1; generation <= clonalg->getNgen(); ++generation){
        clonalg->evaluatePopulation();
        auto clones = clonalg->createClones();
        clonalg->hypermutation(clones);
        clonalg->updateMemory(clones);
        clonalg->applyDiversity();
        clonalg->updatePopulation();


    }

    BuildCSV *csv_builder = new BuildCSV();
    csv_builder->printFacResultsCSV(clonalg, output_file);
    delete csv_builder;
    
    delete clonalg;
}