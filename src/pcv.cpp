#include <bits/stdc++.h>

#include "graph.hpp"
#include "pcv.hpp"

using namespace std;

mt19937 globalGenerator(random_device{}());

void Pcv::initPopulation(){
    for(vector<int> &individual : population){
        iota(individual.begin(), individual.end(), 0);
        shuffle(individual.begin(), individual.end(), globalGenerator);
    }
}

void Pcv::evaluatePopulation(){
    for(int idv = 0; idv < npop; ++idv){
        int c1 = 0;
        int c2 = 1;

        fitness[idv] = 0;
        while(c1 < ngenes){
            if(population[idv][c1] >= ngenes or population[idv][c2] >= ngenes){
                exit(1);
            }
            fitness[idv] += graph.getEdge(population[idv][c1], population[idv][c2]);
            c1 += 1;
            c2 = (c2 + 1) % ngenes;
        }
    }
}

vector<int> Pcv::rouletteSelection(){
    vector<int> parents (npop);

    vector<double> accumulated_fitness (npop + 1, 0);
    for(int i = 1; i < npop + 1; ++i){
        accumulated_fitness[i] = accumulated_fitness[i-1] + 1.0/fitness[i-1];
    }

    uniform_int_distribution<> parent_distribution(accumulated_fitness[1],
                                                   accumulated_fitness[npop]);

    for(auto &parent : parents){
        int r = parent_distribution(globalGenerator);
        auto it_selected = lower_bound(accumulated_fitness.begin(),
                                       accumulated_fitness.end(), r);
        parent = distance(accumulated_fitness.begin(), it_selected) - 1;
    }
    return parents;
}

vector<int> Pcv::tournamentSelection(){
    vector<int> parents (npop);

    uniform_int_distribution<> parent_distribution(0, npop-1);
    uniform_real_distribution<> winner_distribution(0.0, 1.0);

    for(int i = 0; i < npop; ++i){
        int parent_1 = parent_distribution(globalGenerator);
        int parent_2;

        do {
            parent_2 = parent_distribution(globalGenerator);
        } while (parent_1 == parent_2);

        // Obtém a probabilidade do mais apto ser o vencedor
        float r = winner_distribution(globalGenerator);

        if(fitness[parent_1] > fitness[parent_2]){
            parents[i] = parent_2;
            if(r > pwinner){
                parents[i] = parent_1;
            }
        } else {
            parents[i] = parent_1;
            if(r > pwinner){
                parents[i] = parent_2;
            }
        }
    }
    return parents;
}

vector<int> Pcv::parentsSelection(const int selection_id){
    return selection_id == 1 ? tournamentSelection() : rouletteSelection();
}

void Pcv::crossoverPMX(vector<int> &parents){
    uniform_real_distribution<> pcrossover_distribution(0.0, 1.0);
    uniform_int_distribution<> point_distribution(0, ngenes - 1);

    for(int i = 0; i < npop - 1; i += 2){
        float r = pcrossover_distribution(globalGenerator);

        int parent_1_idx = parents[i];
        int parent_2_idx = parents[i+1];

        if(r > pcrossover){
            // Copia os genes dos pais diretamente
            copy(population[parent_1_idx].begin(), population[parent_1_idx].end(),
                 intermediate_population[i].begin());
            copy(population[parent_2_idx].begin(), population[parent_2_idx].end(),
                 intermediate_population[i+1].begin());
            continue;
        }

        // "Renomeia" os indivíduos
        const vector<int> &parent_1 = population[parent_1_idx];
        const vector<int> &parent_2 = population[parent_2_idx];
        vector<int> &child_1 = intermediate_population[i];
        vector<int> &child_2 = intermediate_population[i+1];

        // Obtém os dois pontos de corte
        int point_1 = point_distribution(globalGenerator);
        int point_2 = point_distribution(globalGenerator);
        if(point_1 > point_2) swap(point_1, point_2);

        // Copia a seção entre os pontos de corte
        for(int j = point_1; j <= point_2; ++j){
            child_1[j] = parent_2[j];
            child_2[j] = parent_1[j];
        }

        // Cria os mapeamentos
        map<int, int> map_1, map_2;
        for(int j = point_1; j <= point_2; ++j){
            map_1[parent_2[j]] = parent_1[j];
            map_2[parent_1[j]] = parent_2[j];
        }

        // Preenche os genes antes do primeiro ponto de corte
        for(int j = 0; j < point_1; ++j){
            // Filho 1
            int gene = parent_1[j];
            while(map_1.find(gene) != map_1.end())
                gene = map_1[gene];
            child_1[j] = gene;

            // Filho 2
            gene = parent_2[j];
            while(map_2.find(gene) != map_2.end())
                gene = map_2[gene];
            child_2[j] = gene;
        }

        // Preenche os genes depois do segundo ponto de corte
        for(int j = point_2 + 1; j < ngenes; ++j){
            // Filho 1
            int gene = parent_1[j];
            while(map_1.find(gene) != map_1.end())
                gene = map_1[gene];
            child_1[j] = gene;

            // Filho 2
            gene = parent_2[j];
            while(map_2.find(gene) != map_2.end())
                gene = map_2[gene];
            child_2[j] = gene;
        }
    }
}

void Pcv::crossoverOX(vector<int> &parents){
    uniform_real_distribution<> pcrossover_distribution(0.0, 1.0);
    uniform_int_distribution<> point_distribution(0, ngenes - 1);

    for(int i = 0; i < npop - 1; i+=2){
        float r = pcrossover_distribution(globalGenerator);

        int parent_1 = parents[i];
        int parent_2 = parents[i+1];

        if(r > pcrossover){
            // Copia os genes dos pais diretamente
            copy(population[parent_1].begin(), population[parent_1].end(),
                 intermediate_population[i].begin());
            copy(population[parent_2].begin(), population[parent_2].end(),
                 intermediate_population[i+1].begin());
            continue;
        }

        // Obtém os dois pontos de corte
        int point_1 = point_distribution(globalGenerator);
        int point_2 = point_distribution(globalGenerator);
        if(point_1 > point_2) swap(point_1, point_2);

        set<int> son_1, son_2;
        for(int j = point_1; j <= point_2; ++j){
            intermediate_population[i][j] = population[parent_2][j];
            son_1.insert(population[parent_2][j]);

            intermediate_population[i+1][j] = population[parent_1][j];
            son_2.insert(population[parent_1][j]);
        }

        int j = (point_2 + 1) % ngenes;
        int j1 = j, j2 = j;

        while(j != point_1){
            while(son_1.find(population[parent_1][j1]) != son_1.end()){
                son_1.erase(son_1.find(population[parent_1][j1]));
                j1 = (j1+1) % ngenes;
            }

            while(son_2.find(population[parent_2][j2]) != son_2.end()){
                son_2.erase(son_2.find(population[parent_2][j2]));
                j2 = (j2+1) % ngenes;
            }

            intermediate_population[i][j] = population[parent_1][j1];
            intermediate_population[i+1][j] = population[parent_2][j2];

            j  = (j+1)  % ngenes;
            j1 = (j1+1) % ngenes;
            j2 = (j2+1) % ngenes;
        }
    }
}

void Pcv::crossover(vector<int> &parents, int crossover_id){
    if(crossover_id == 1){
        crossoverOX(parents);
    } else if (crossover_id == 2){
        crossoverPMX(parents);
    } else {
        cerr << "Crossover ID invalido\n";
        exit(1);
    }
}

void Pcv::mutation(){
    uniform_real_distribution<> mutation_distribution(0.0, 1.0);
    uniform_int_distribution<> gene_distribution(0, ngenes - 1);

    auto it_local_min = min_element(fitness.begin(), fitness.end());
    if(*it_local_min < local_min){
        local_min = *it_local_min;
        extra_pmutation = 0.0;
        count_gen_local_min = 0;
    } else {
        count_gen_local_min += 1;
        if(count_gen_local_min >= 10){
            extra_pmutation = 0.10;
        }
    }

    for(int i = 0; i < npop; ++i){
        for(int j = 0; j < ngenes; ++j){
            float r = mutation_distribution(globalGenerator);

            if(r <= (pmutation + extra_pmutation)){
                int gene_idx;
                do {
                    gene_idx = gene_distribution(globalGenerator);
                } while (gene_idx == j);

                swap(intermediate_population[i][j],
                     intermediate_population[i][gene_idx]);
            }
        }
    }
}

void Pcv::copyPopulation(){
    for(int i = 0; i < npop; ++i){
        copy(intermediate_population[i].begin(), intermediate_population[i].end(),
             population[i].begin());
    }
}

void Pcv::elitism(){
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;
    for(int i = 0; i < npop; ++i){
        min_heap.push(make_pair(fitness[i], i));
    }

    for(int i = 0; i < nelite; ++i){
        pair<int, int> elite_idv = min_heap.top();
        min_heap.pop();

        copy(population[elite_idv.second].begin(),
             population[elite_idv.second].end(),
             intermediate_population[i].begin());
    }
}

void Pcv::printResults() const {
    auto it_shortest_path = min_element(fitness.begin(), fitness.end());
    int shortest_path_id = distance(fitness.begin(), it_shortest_path);

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "                  Resumo dos Resultados                    " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << " Menor distancia: " << *it_shortest_path << endl;
    cout << " Caminho: ";
    for(int i = 0; i < ngenes; ++i)
        cout << population[shortest_path_id][i] << " -> ";
    cout << population[shortest_path_id][0] << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
}

void Pcv::printPopulation() const {
    for(int i = 0; i < npop; ++i){
        cout << "Percurso " << i+1 << ":" << endl;
        cout << "\tDistancia: " << fitness[i] << endl;
        cout << "\tCaminho: ";
        for(int j = 0; j < ngenes; ++j)
            cout << population[i][j] << " -> ";
        cout << population[i][0] << endl << endl;
    }
}

Pcv::Pcv(const Graph &_graph, int _npop, int _ngen, int _ngenes, int _nelite,
         double _pcrossover, double _pmutation, double _pwinner) :
    graph(_graph), npop(_npop), ngen(_ngen), ngenes(_ngenes), nelite(_nelite),
    pcrossover(_pcrossover), pmutation(_pmutation), local_min(INT_MAX),
    extra_pmutation(0.0), count_gen_local_min(0),
    population(_npop, vector<int> (_ngenes)),
    intermediate_population(_npop, vector<int> (_ngenes)), fitness(_npop),
    pwinner(_pwinner) {}

Pcv::~Pcv() {}
