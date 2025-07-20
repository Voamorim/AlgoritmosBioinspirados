#include "solve.hpp"
#include "ant_system.hpp"

#include <omp.h>

void Solve::solve(ifstream &input_file){
    const int npop = 200;
    const int num_iterations = 200;
    const int Q = 100;
    const double alfa = 1.5;
    const double beta = 1.3;
    const double p = 0.5;

    Graph graph = Graph();
    graph.readGraph(input_file);
    input_file.close();

    AntSystem *ant_system = new AntSystem(graph, num_iterations, npop, Q, alfa,
                                          beta, p);

    vector<int> ants_start(ant_system->npop);
    ant_system->buildAntsStart(ants_start);
    ant_system->initPheromones();

    vector<int> min_cycle;
    double min_cost = 0;

    bool changed = false;
    int count_not_changed = 0;
    int max_not_changed = ceil(sqrt(ant_system->num_iterations));

    for(int generation = 0; generation < ant_system->num_iterations; ++generation){
        Graph *used_costs = new Graph(graph.getNumVertices());
        for(int ant = 0; ant < ant_system->npop; ++ant){
            pair<vector<int>, double> cycle_info;
            cycle_info = ant_system->buildCycle(ants_start[ant]);

            ant_system->updateUsedCosts(used_costs, cycle_info);

            if(cycle_info.second < min_cost or (generation == 0 and ant == 0)){
                min_cost = cycle_info.second;
                min_cycle = cycle_info.first;

                changed = true;
            }
        }
        if(changed) count_not_changed = 0;
        else count_not_changed += 1;

        changed = false;

        if(max_not_changed < count_not_changed) break;

        ant_system->updatePheromones(used_costs);
        delete used_costs;
    }

    ant_system->printCycle(min_cycle, min_cost);

    delete ant_system;
}

void Solve::factorialTest(ifstream &input_file, ofstream &output_file){
    const int npop[] = {50, 100, 200};
    const int Q = 100;
    const double alfa[] = {1.0, 1.3, 1.5};
    const double beta[] = {1.0, 1.3, 1.5};
    const double p[] = {0.75, 0.5, 0.9};

    Graph graph = Graph();
    graph.readGraph(input_file);

    input_file.close();

    BuildCSV *build_csv = new BuildCSV();
    build_csv->printFacIdfCSV(output_file);
    delete build_csv;

    int count = 0;

    for(int ipop = 0; ipop < 3; ++ipop){
        for(int ialfa = 0; ialfa < 3; ++ialfa){
            for(int ibeta = 0; ibeta < 3; ++ibeta){
                for(int ip = 0; ip < 3; ++ip){
                    int ngen = npop[ipop];
                    solveFactorialTest(graph, ngen,
                            npop[ipop], Q, alfa[ialfa], beta[ibeta],
                            p[ip], output_file);
                    count ++;
                    cout << count << endl;
                }
            }
        }
    }

    output_file.close();
}

void Solve::generationsTest(ifstream &input_file, ofstream &output_file){
    const int npop = 200;
    const int num_iterations = 200;
    const int Q = 100;
    const double alfa = 1.5;
    const double beta = 1.3;
    const double p = 0.5;

    Graph graph = Graph();
    graph.readGraph(input_file);
    input_file.close();

    solveGenerationsTest(graph, num_iterations, npop, Q, alfa, beta, p, output_file);

    output_file.close();
}

void Solve::solveFactorialTest(const Graph &graph, const int num_iterations,
                               const int npop, const int Q, const double alfa,
                               const double beta, const double p,
                               ofstream &output_file){
    BuildCSV *build_csv = new BuildCSV();

    AntSystem *ant_system = new AntSystem(graph, num_iterations, npop, Q, alfa, beta,
                                          p);

    vector<int> ants_start(ant_system->npop);
    ant_system->buildAntsStart(ants_start);
    ant_system->initPheromones();

    vector<int> global_best_cycle;
    double global_best_cost = DBL_MAX;
    omp_lock_t best_solution_lock;
    omp_init_lock(&best_solution_lock);

    for (int generation = 0; generation < ant_system->num_iterations; ++generation) {
        double generation_sum = 0.0;
        Graph *used_costs_gen = new Graph(graph.getNumVertices());

        #pragma omp parallel reduction(+:generation_sum)
        {
            vector<int> thread_best_cycle;
            double thread_best_cost = DBL_MAX;
            Graph *used_costs_local = new Graph(graph.getNumVertices());

            #pragma omp for nowait
            for (int ant = 0; ant < ant_system->npop; ++ant) {
                pair<vector<int>, double> cycle_info;
                cycle_info = ant_system->buildCycle(ants_start[ant]);

                if (cycle_info.second >= INT_MAX) continue;

                ant_system->updateUsedCosts(used_costs_local, cycle_info);
                generation_sum += cycle_info.second;

                if (cycle_info.second < thread_best_cost) {
                    thread_best_cost = cycle_info.second;
                    thread_best_cycle = cycle_info.first;
                }
            }

            #pragma omp critical
            {
                for (int i = 0; i < graph.getNumVertices(); i++) {
                    for (int j = 0; j < graph.getNumVertices(); j++) {
                        double current = used_costs_gen->getEdge(i, j);
                        double add = used_costs_local->getEdge(i, j);
                        used_costs_gen->setEdge(i, j, current + add);
                    }
                }
            }

            if (thread_best_cost < global_best_cost) {
                omp_set_lock(&best_solution_lock);
                if (thread_best_cost < global_best_cost){
                    global_best_cost = thread_best_cost;
                    global_best_cycle = thread_best_cycle;
                }
                omp_unset_lock(&best_solution_lock);
            }

            delete used_costs_local;
        }

        ant_system->sum_costs += generation_sum;
        if (global_best_cost < ant_system->min_cycle) {
            ant_system->min_cycle = global_best_cost;
        }

        ant_system->updatePheromones(used_costs_gen);
        delete used_costs_gen;
    }

    omp_destroy_lock(&best_solution_lock);
    build_csv->printFacResultsCSV(ant_system, output_file);

    delete build_csv;
    delete ant_system;
}

void Solve::solveGenerationsTest(const Graph &graph, const int num_iterations,
                                 const int npop, const int Q, const double alfa,
                                 const double beta, const double p,
                                 ofstream &output_file){
    AntSystem *ant_system = new AntSystem(graph, num_iterations, npop, Q, alfa, beta,
                                          p);

    BuildCSV *build_csv = new BuildCSV();
    build_csv->printGenIdfCSV(ant_system, output_file);
    delete build_csv;

    vector<int> ants_start(ant_system->npop);
    ant_system->buildAntsStart(ants_start);
    ant_system->initPheromones();

    for(int generation = 0; generation < ant_system->num_iterations; ++generation){
        Graph *used_costs = new Graph(graph.getNumVertices());
        output_file << generation << ',';
        for(int ant = 0; ant < ant_system->npop; ++ant){
            pair<vector<int>, double> cycle_info;
            cycle_info = ant_system->buildCycle(ants_start[ant]);

            ant_system->updateUsedCosts(used_costs, cycle_info);

            output_file << cycle_info.second;
            if(ant != ant_system->npop - 1)
                output_file << ',';
        }
        output_file << '\n';
        ant_system->updatePheromones(used_costs);
        delete used_costs;
    }

    delete ant_system;
}
