#ifndef CLONALG_HPP
#define CLONALG_HPP

#include <bits/stdc++.h>

#include "graph.hpp"

using namespace std;

typedef unsigned uint;

class Clonalg{
	private:
		const Graph *graph;

		// Cada indivíduo guarda um par <ciclo, custo>
		vector<pair<vector<uint>, double>> population;
		vector<pair<double, uint>> fitness; 
		vector<pair<vector<uint>, double>> memory;

		uint npop; // População total
		uint nclones; // Número de anticorpos a serem clonados
		uint ngen; // Quantidade de gerações
		uint d; // Quantidade de anticorpos a serem substituídos	

	    double beta; // Fator de clonagem
		double rho;

		uint getStartingVertex(void);
		void evaluateIndividual(pair<vector<uint>, double> &indv);
		pair<vector<uint>, double> getMinIndividualPopulation(void);
	public:
		void createInitialPopulation(void);	
		void evaluatePopulation(void);
		vector<pair<vector<uint>, double>> createClones(void);
		void hypermutation(vector<pair<vector<uint>, double>> &clones);
		vector<pair<double, uint>> evaluateClones 
						  (vector<pair<vector<uint>, double>> &clones);
		void updateMemory(vector<pair<vector<uint>, double>> &clones);
		void applyDiversity(void);
		void updatePopulation(void);

		uint getNgen(void) const;
		uint getNpop(void) const;
		uint getNclones(void) const;
		uint getD(void) const;
		double getBeta(void) const;
		double getRho(void) const;
		double getMinIndividualCost(void);
		double getPopulationAverageCost(void);
		vector<pair<vector<uint>, double>> getPopulation(void) const;

		void printResults(void);

		Clonalg(const Graph *_graph, uint _npop, uint _nclones, uint _ngen,
				uint _d, double _beta, double _rho);
		~Clonalg();
};

#endif
