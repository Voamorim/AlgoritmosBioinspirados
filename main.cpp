#include "solve.hpp"
#include "io.hpp"

#include <bits/stdc++.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

int main(int argc, char **argv){ _
	Io *io = new Io();

	int solution_id = io->getSolutionId(argc, argv);

	Solve solve;

	switch(solution_id){
		case 1:{
			solve.solve();
			break;
		}
		case 2:{
			ofstream gen_output_file(io->getGenOutputFileName(argc, argv));

			if(not gen_output_file.is_open()){
				cerr << "Erro ao abrir o arquivo de saida do teste de " <<
					    "geracoes." << endl;
				return 1;
			}

			solve.generationsTest(gen_output_file);
			break;
		}
		case 3:{
			ofstream fac_output_file(io->getFacOutputFileName(argc, argv));

			if(not fac_output_file.is_open()){
				cerr << "Erro ao abrir o arquivo de saida do teste fatorial.\n";
				return 1;
			}

			solve.factorialTest(fac_output_file);
			break;
		}
	};

	return 0;
} 
