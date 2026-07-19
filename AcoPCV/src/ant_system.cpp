#include "ant_system.hpp"

thread_local mt19937 globalGenerator(random_device{}());

AntSystem::AntSystem(const Graph &_graph, const int _num_iterations,
                     const int _npop, const int _Q, const double _alfa,
                     const double _beta, const double _p) :
    graph(_graph), num_iterations(_num_iterations), npop(_npop), Q(_Q),
    alfa(_alfa), beta(_beta), p(_p), min_cycle(INT_MAX), sum_costs(0) {}

AntSystem::~AntSystem() {}

int AntSystem::getStartingVertex(void){
    uniform_int_distribution<> dis(0, graph.getNumVertices() - 1);
    return dis(globalGenerator);
}

void AntSystem::buildAntsStart(vector<int> &ants_start){
    for(int &ant : ants_start){
        ant = getStartingVertex();
    }
}

void AntSystem::initPheromones(void){
    pheromones.initGraph(graph.getNumVertices());
    for(int i = 0; i < graph.getNumVertices(); ++i){
        for(int j = 0; j < graph.getNumVertices(); ++j){
            pheromones.setEdge(i, j, STARTING_PHEROMONES);
        }
    }
}

void AntSystem::updatePheromones(const Graph *used_costs){
    for(int i = 0; i < graph.getNumVertices(); ++i){
        for(int j = 0; j < graph.getNumVertices(); ++j){
            double new_value = (1.0 - p) * pheromones.getEdge(i, j) +
                               used_costs->getEdge(i, j);

            pheromones.setEdge(i, j, new_value);
        }
    }
}

double AntSystem::neighborsProbabilitySum(const vector<bool> &visited,
                                          const int vertex) {
    double sum = 0;

    for(int neighbor = 0; neighbor < graph.getNumVertices(); ++neighbor){
        if(visited [neighbor] or not graph.getEdge(vertex, neighbor))
            continue;

        double t = pheromones.getEdge(vertex, neighbor);
        double inv_cost = 1.0 / graph.getEdge(vertex, neighbor);

        double first_term = pow(t, alfa);
        double second_term = pow(inv_cost, beta);

        sum += first_term * second_term;
    }
    return sum;
}

int AntSystem::getRandomWithWeights(const vector<double> &probabilities){
    vector<double> cumulative;
    vector<int> elements;

    cumulative.clear();
    elements.clear();
    double sum = 0.0;

    for(int i = 0; i < (int) probabilities.size(); ++i){
        if(probabilities[i] > 0){
            sum += probabilities[i];
            cumulative.push_back(sum);
            elements.push_back(i);
        }
    }
    if(elements.size() == 1) return elements[0];

    uniform_real_distribution<double> dist(0.0, sum);

    double r = dist(globalGenerator);
    auto it = lower_bound(cumulative.begin(), cumulative.end(), r);
    return elements[distance(cumulative.begin(), it)];
}

int AntSystem::getNextVertex(const vector<bool> &visited, const int vertex){
    vector<double> probabilities(graph.getNumVertices(), 0.0);
    double neighbors_sum = neighborsProbabilitySum(visited, vertex);

    for(int neighbor = 0; neighbor < graph.getNumVertices(); ++neighbor){
        if(visited[neighbor] or not graph.getEdge(vertex, neighbor)) continue;

        double t = pheromones.getEdge(vertex, neighbor);
        double inv_cost = 1.0 / graph.getEdge(vertex, neighbor);

        double first_term = pow(t, alfa);
        double second_term = pow(inv_cost, beta);

        probabilities[neighbor] = (first_term * second_term) / neighbors_sum;
    }

    int next_vertex = getRandomWithWeights(probabilities);

    if(next_vertex < 0 or next_vertex > graph.getNumVertices()){
        cerr << "ant_system.cpp: Erro ao obter o proximo vertice do ciclo em " <<
                "getNextVertex(). O vertice " << next_vertex << "nao faz parte " <<
                "do grafo." << endl;
        exit(1);
    }

    return next_vertex;
}

pair<vector<int>, double> AntSystem::buildCycle(const int start){
    vector<int> cycle;
    vector<bool> visited(graph.getNumVertices(), false);

    cycle.reserve(graph.getNumVertices() + 1);
    cycle.push_back(start);
    visited[start] = true;

    double cost = 0;

    while((int) cycle.size() < graph.getNumVertices()){
        int current_vertex = cycle.back();
        int next_vertex = getNextVertex(visited, current_vertex);

        cycle.push_back(next_vertex);

        int edge_cost = graph.getEdge(current_vertex, next_vertex);
        cost += edge_cost;
        visited[next_vertex] = true;
    }

    int starting_vertex = cycle[0];
    int last_vertex = cycle[cycle.size() - 1];
    double edge_cost = graph.getEdge(last_vertex, starting_vertex);
    cost += edge_cost;
    cycle.push_back(starting_vertex);

    return make_pair(cycle, cost);
}

void AntSystem::updateUsedCosts(Graph *used_costs,
                                const pair<vector<int>, double> &cycle_info){
    for(int i = 0, j = 1; j < (int) cycle_info.first.size(); ++i, ++j){
        int src = cycle_info.first[i];
        int dest = cycle_info.first[j];

        double increment = (double) Q/cycle_info.second;
        used_costs->incrementEdge(src, dest, increment);
        used_costs->incrementEdge(dest, src, increment);
    }
}

void AntSystem::printCycle(const vector<int> &cycle, const double min_cost) {
    cout << "Ciclo de menor custo encontrado:" << endl;
    cout << cycle[0];
    for(int v = 1; v < (int) cycle.size(); ++v){
        cout << " -> " << cycle[v];
    }
    cout << endl;
    cout << fixed << setprecision(2) << "Custo: " << min_cost << endl;
}
