#include "solve.hpp"
#include "pcv.hpp"
#include "build_csv.hpp"
#include "graph.hpp"

#include <bits/stdc++.h>

using namespace std;

void Solve::solve(ifstream &input_file, const int crossover_id,
                  const int selection_id){
    const int npop = 200;
    const int ngen = 200;
    const int nelite = 6;
    const double pcrossover = 0.9;
    const double pwinner = 0.9;
    const double pmutation = 0.01;

    Graph graph;
    graph.readGraph(input_file);
    input_file.close();

    const int ngenes = graph.n;

    Pcv *pcv = new Pcv(graph, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                       pwinner);

    pcv->initPopulation();

    for(int generation = 0; generation < pcv->ngen; ++generation){
        pcv->evaluatePopulation();

        vector<int> parents = pcv->parentsSelection(selection_id);

        pcv->crossover(parents, crossover_id);

        pcv->mutation();
        pcv->elitism();

        pcv->copyPopulation();
        //pcv->printResults();
    }

    pcv->printResults();

    delete pcv;
}

void Solve::factorialTest(ifstream &input_file, ofstream &output_file,
                          const int crossover_id, const int selection_id){
    const int vnpop[] = {100, 150, 200};
    const int vngen[] = {100, 150, 200};
    const int vnelite[] = {2, 4, 6};
    const double vpcrossover[] = {0.8, 0.9, 1.0};
    const double pwinner = 0.9;
    const double vpmutation[] = {0.01, 0.05, 0.10};

    Graph graph;
    graph.readGraph(input_file);
    input_file.close();

    const int ngenes = graph.n;

    BuildCSV csv_builder;
    csv_builder.printFacIdfCSV(output_file);

    for(int ipop = 0; ipop < 3; ++ipop){
        for(int igen = 0; igen < 3; ++igen){
            for(int ielite = 0; ielite < 3; ++ielite){
                for(int icrossover = 0; icrossover < 3; ++icrossover){
                    for(int imutation = 0; imutation < 3; ++imutation){
                        solveFactorialTest(graph, vnpop[ipop], vngen[igen], ngenes,
                                           vnelite[ielite], vpcrossover[icrossover],
                                           vpmutation[imutation], pwinner,
                                           output_file, crossover_id, selection_id);
                    }
                }
            }
        }
    }

    output_file.close();
}

void Solve::solveFactorialTest(Graph &graph, int npop, int ngen, int ngenes,
                               int nelite, double pcrossover, double pmutation,
                               double pwinner, ofstream &output_file,
                               const int crossover_id, const int selection_id){
    Pcv *pcv = new Pcv(graph, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                       pwinner);

    BuildCSV csv_builder;

    pcv->initPopulation();
    for(int generation = 0; generation < pcv->ngen; ++generation){
        pcv->evaluatePopulation();

        vector<int> parents = pcv->parentsSelection(selection_id);
        pcv->crossover(parents, crossover_id);

        pcv->mutation();
        pcv->elitism();

        pcv->copyPopulation();
    }

    csv_builder.printFacResultsCSV(pcv, output_file);
    delete pcv;
}

void Solve::generationsTest(ifstream &input_file, ofstream &output_file,
                            const int crossover_id, const int selection_id){
    const int npop = 150;
    const int ngen = 150;
    const int nelite = 6;
    const double pcrossover = 0.9;
    const double pwinner = 0.9;
    const double pmutation = 0.01;

    Graph graph;
    graph.readGraph(input_file);
    input_file.close();

    const int ngenes = graph.n;

    solveGenerationsTest(graph, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                         pwinner, output_file, crossover_id, selection_id);

    output_file.close();
}

void Solve::solveGenerationsTest(Graph &graph, int npop, int ngen, int ngenes,
                                 int nelite, double pcrossover, double pmutation,
                                 double pwinner, ofstream &output_file,
                                 const int crossover_id, const int selection_id){
    Pcv *pcv = new Pcv(graph, npop, ngen, ngenes, nelite, pcrossover, pmutation,
                       pwinner);

    BuildCSV csv_builder;

    csv_builder.printGenIdfCSV(pcv, output_file);
    pcv->initPopulation();
    for(int generation = 0; generation < pcv->ngen; ++generation){
        pcv->evaluatePopulation();

        vector<int> parents = pcv->parentsSelection(selection_id);
        pcv->crossover(parents, crossover_id);

        pcv->mutation();
        pcv->elitism();

        pcv->copyPopulation();

        csv_builder.printGenResultsCSV(pcv, output_file, generation);
    }
    delete pcv;
}
