#ifndef CLONALG_HPP
#define CLONALG_HPP

#include <bits/stdc++h>

using namespace std;

class Clonalg{
	private:
		Graph *graph;

		unsigned N; // População total
		unsigned n; // Número de anticorpos a serem clonados
	
		unsigned ngen; // Quantidade de gerações

		unsigned d; // Quantidade de anticorpos a serem substituídos
		
	      	double beta; // Fator de clonagem

		// Calcula o custo individualmente do ciclo
		double evaluateIndividual(const int individual);	


	public:
		// Cria uma população inicial de forma gulosa ou aleatória
		void createInitialPopulation();	

		void evaluatePopulation();
};

#endif
