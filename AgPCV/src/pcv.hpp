#ifndef PCV_HPP
#define PCV_HPP

#include "graph.hpp"

class Pcv{
    public:
        Graph graph;

        int npop;
        int ngen;
        int ngenes;
        int nelite;
        double pcrossover;
        double pmutation;

        double local_min;
        double extra_pmutation;
        int count_gen_local_min;

        vector<vector<int>> population;
        vector<vector<int>> intermediate_population;
        vector<int> fitness;

        double pwinner;

        void initPopulation();
        void evaluatePopulation();
        vector<int> parentsSelection(const int selection_id);
        vector<int> tournamentSelection();
        vector<int> rouletteSelection();
        void crossoverPMX(vector<int> &parents);
        void crossoverOX(vector<int> &parents);
        void crossover(vector<int> &parents, int crossover_id);
        void mutation();
        void copyPopulation();
        void elitism();
        void printResults() const;
        void printPopulation() const;

        Pcv(const Graph &_graph, int _npop, int _ngen, int _ngenes, int _nelite,
            double _pcrossover, double _pmutation, double _pwinner);
        ~Pcv();
};

#endif
