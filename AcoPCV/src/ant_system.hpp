#ifndef ANT_SYSTEM_HPP
#define ANT_SYSTEM_HPP

#include "graph.hpp"

#include <bits/stdc++.h>

using namespace std;

class AntSystem{
    public:
        Graph graph;
        Graph pheromones;

        int num_iterations;
        int npop;
        int Q;
        const double STARTING_PHEROMONES = 1e-16;
        double alfa;
        double beta;
        double p;

        double min_cycle;
        long double sum_costs;

        int getStartingVertex(void);

        void buildAntsStart(vector<int> &ants_start);

        void initPheromones(void);

        void updatePheromones(const Graph *used_edges);

        double neighborsProbabilitySum(const vector<bool> &visited,
                                       const int vertex);

        int getRandomWithWeights(const vector<double> &probabilities);

        int getNextVertex(const vector<bool> &visited, const int vertex);

        pair<vector<int>, double> buildCycle(const int start);

        void updateUsedCosts(Graph *used_costs,
                             const pair<vector<int>, double> &cycle_info);

        void printCycle(const vector<int> &cycle, const double min_cost);

        AntSystem(const Graph &_graph, int _num_iterations, int _npop, int _Q,
                  double _alfa, double _beta, double _p);
        ~AntSystem();
};

#endif
