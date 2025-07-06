#include "clonalg.hpp"

mt19937 globalGenerator(random_device{}());

#define dbg(x) cout << #x << " == " << x << endl

Clonalg::Clonalg(const Graph *_graph, uint _npop, uint _nclones,
                 uint _ngen, uint _d, double _beta, double _rho) :
    graph(_graph), population(_npop), fitness(_npop), memory(_nclones),
    npop(_npop), nclones(_nclones), ngen(_ngen), d(_d), beta(_beta),
    rho(_rho) {}

Clonalg::~Clonalg() {}

uint Clonalg::getStartingVertex(void){
    uniform_int_distribution<> dis(0, graph->getNumVertices() - 1);
    return dis(globalGenerator);
}

void Clonalg::createInitialPopulation(uint strategy_id){
    switch (strategy_id){
        case 1: {
            createRandomInitialPopulation();
            break;
        }
        case 2:{
            createGreedyInitialPopulation();
            break;
        }
    }
}

void Clonalg::createRandomInitialPopulation(){
    for(uint i = 0; i < npop; ++i){
        fitness[i].second = i;

        uint src = getStartingVertex();

        population[i].first.push_back(src);
        population[i].second = 0.0;

        uint n = graph->getNumVertices();

        set<uint> visited_vertices;
        visited_vertices.insert(src);

        uniform_int_distribution<> dis(0, n - 1);
        while(visited_vertices.size() != n){
            uint u;

            do{
                u = dis(globalGenerator);
            } while(visited_vertices.find(u) != visited_vertices.end());

            population[i].first.push_back(u);
            population[i].second += graph->getEdge(src, u);

            src = u;
            visited_vertices.insert(u);
        }
        population[i].second += graph->getEdge(population[i].first[n-1],
                                               population[i].first[0]);
        population[i].first.push_back(population[i].first[0]);
    }
}

// Cria população inicial com heurística gulosa
void Clonalg::createGreedyInitialPopulation(){
    for(uint i = 0; i < npop; ++i){
        fitness[i].second = i;

        uint src = getStartingVertex();

        population[i].first.push_back(src);
        population[i].second = 0.0;

        uint n = graph->getNumVertices();

        set<uint> visited_vertices;
        visited_vertices.insert(src);

        for (uint k = 1; k < n; ++k){
            pair<double, uint> min_cost = make_pair(DBL_MAX, -1);

            for (uint j = 0; j < n; ++j){
                if (j == src)
                    continue;

                double cost = graph->getEdge(src, j);
                if (cost < min_cost.first and
                    visited_vertices.find(j) == visited_vertices.end())
                    min_cost = make_pair(cost, j);
            }

            population[i].first.push_back(min_cost.second);
            population[i].second += min_cost.first;

            src = min_cost.second;
            visited_vertices.insert(src);
        }
        population[i].second += graph->getEdge(population[i].first[n-1],
                                               population[i].first[0]);
        population[i].first.push_back(population[i].first[0]);
    }
}

void Clonalg::evaluateIndividual(pair<vector<uint>, double> &indv){
    indv.second = 0.0;

    uint n = graph->getNumVertices();
    for (uint i = 0, j = 1; i < n; ++i, j = (j + 1) % n){
        //if(indv.first[i] > 128 or indv.first[j] > 128)
        //    dbg(indv.first[i]), dbg(indv.first[j]), dbg(i), dbg(j);
        indv.second += graph->getEdge(indv.first[i], indv.first[j]);
    }
}

void Clonalg::evaluatePopulation(void){
    for(uint i = 0; i < npop; ++i){
        auto& indv = population[fitness[i].second];

        evaluateIndividual(indv);

        fitness[i].first = max(min(1.0 / indv.second, 1.0), 0.0);
    }
}

vector<pair<vector<uint>, double>> Clonalg::createClones(void){
    // Ordena os fitness de forma decrescente
    sort(fitness.begin(), fitness.end(), greater<pair<double, uint>>());

    vector<pair<vector<uint>, double>> clones;

    uint i = 0;
    while(i < nclones){
        uint nclones_idv = (beta * npop)/(i + 1);

        auto idv = population[fitness[i].second];
        for(uint j = 0; j < nclones_idv; ++j){
            clones.push_back(idv);
        }
        i++;
    }

    return clones;
}

void Clonalg::hypermutation(vector<pair<vector<uint>, double>> &clones){
    auto clones_fitness = evaluateClones(clones);

    auto it = max_element(clones_fitness.begin(), clones_fitness.end(),
                          [](const auto& a , const auto& b){
        return a.second < b.second;
    });

    if(it == clones_fitness.end()){
        cerr << "clonalg.cpp: Erro ao obter o clone de maior distancia total " <<
                "em hypermutation()." << endl;
        exit(EXIT_FAILURE);
    }

    double D_max = (*it).second;

    for(uint i = 0; i < clones.size(); ++i){
        auto& clone = clones[i];

        double D_star = clone.second / D_max;

        double alpha = exp(-rho * D_star);

        for(uint j = 0; j < clone.first.size(); ++j){
            uniform_real_distribution<> r_dis(0.0, 1.0);
            double r = r_dis(globalGenerator);

            // Caso o valor sorteado seja menor que a probabilidade de mutação
            if(r <= alpha){
                uniform_int_distribution<> swap_dis(0, clone.first.size() - 1);

                uint swap_idx;
                do{
                    swap_idx = swap_dis(globalGenerator);
                } while(j == swap_idx);

                swap(clone.first[j], clone.first[swap_idx]);
            }
        }
    }
}

vector<pair<double, uint>> Clonalg::
                evaluateClones(vector<pair<vector<uint>, double>> &clones){
    vector<pair<double, uint>> clones_fitness(clones.size());

    for(uint i = 0; i < clones.size(); ++i){
        auto& clone = clones[i];

        evaluateIndividual(clone);

        clones_fitness[i].first = max(min(1.0 / clone.second, 1.0), 0.0);
        clones_fitness[i].second = i;
    }

    return clones_fitness;
}

void Clonalg::updateMemory(vector<pair<vector<uint>, double>> &clones){
    auto clones_fitness = evaluateClones(clones);

    // Ordena os fitness de forma decrescente
    sort(clones_fitness.begin(), clones_fitness.end(), greater<pair<double, uint>>());

    // Salva os nclones melhores clones na memória
    for(uint i = 0; i < nclones; ++i){
        uint clone_idx = clones_fitness[i].second;
        memory[i] = clones[clone_idx];
    }
}

void Clonalg::applyDiversity(void){
    for(uint i = npop - 1; i > (npop - 1) - d; --i){
        auto& indv = population[fitness[i].second];

        uint src = getStartingVertex();
        uint n = graph->getNumVertices();

        set<uint> inserted_vertices;
        inserted_vertices.insert(src);

        indv.first[0] = src;
        indv.second = 0.0;

        for(uint j = 1; j < n; ++j){
            uniform_int_distribution<> v_dis(0, n-1);

            uint new_v;
            do {
                new_v = v_dis(globalGenerator);
            } while(inserted_vertices.find(new_v) != inserted_vertices.end());

            inserted_vertices.insert(new_v);

            indv.first[j] = new_v;

            indv.second += graph->getEdge(src, new_v);
            src = new_v;
        }

        indv.first[n] = indv.first[0];
        indv.second += graph->getEdge(indv.first[n-1], indv.first[n]);
    }
}

void Clonalg::updatePopulation(void){
    for(uint i = 0; i < nclones; ++i){
        population[i] = memory[i];
    }
}

uint Clonalg::getNgen(void) const{
    return ngen;
}

uint Clonalg::getNpop(void) const{
    return npop;
}

uint Clonalg::getNclones(void) const{
    return nclones;
}

uint Clonalg::getD(void) const{
    return d;
}

double Clonalg::getBeta(void) const{
    return beta;
}

double Clonalg::getRho(void) const{
    return rho;
}

double Clonalg::getMinIndividualCost(void){
    auto min_cycle = getMinIndividualPopulation();
    return min_cycle.second;
}

double Clonalg::getPopulationAverageCost(void){
    double total_cost = 0.0;
    for(auto &idv : population){
        total_cost += idv.second;
    }
    return total_cost / npop;
}

vector<pair<vector<uint>, double>> Clonalg::getPopulation(void) const{
    return population;
}

pair<vector<uint>, double> Clonalg::getMinIndividualPopulation(void){
    pair<vector<uint>, double> min_individual;
    min_individual.second = DBL_MAX;

    for(auto &idv : population){
        if(idv.second < min_individual.second){
            min_individual = idv;
        }
    }

    return min_individual;
}

void Clonalg::printResults(void){
    auto min_individual = getMinIndividualPopulation();

    cout << fixed << setprecision(2);

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "       Algoritmo de Seleção Clonal (CLONALG) para        " << endl;
    cout << "     minimizar o Problema do Caixeiro Viajante (PCV)     " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "- Custo do menor ciclo encontrado: " << min_individual.second << endl;
    cout << "- Ciclo: " << min_individual.first[0];
    for(uint i = 1; i < min_individual.first.size(); i++){
        cout << " -> " << min_individual.first[i];
    }
    cout << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
}
