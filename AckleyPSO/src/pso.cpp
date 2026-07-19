#include "pso.hpp"
#include "particle.hpp"

#define dbg(x) cout << #x << " == " << x << endl

mt19937 globalGenerator(random_device{}());

void Pso::createInitialPopulation(void){
    for(int p = 0; p < npop; ++p){
        Particle *particle = new Particle(ndimensions);

        vector<long double> pos(ndimensions);
        vector<long double> velocity(ndimensions, 0.0);

        uniform_real_distribution<long double> pos_gen(MIN_X, MAX_X);
        uniform_real_distribution<long double> vel_gen(MIN_VELOCITY, MAX_VELOCITY);

        for(int dimension = 0; dimension < ndimensions; ++dimension){
            pos[dimension] = pos_gen(globalGenerator);
        }
        particle->setPos(pos);
        particle->setBestPos(pos);
        particle->setVelocity(velocity);
        
        particle->updateFitness(ackley(pos, ndimensions));  

        // Caso a particula tenha um fitness menor que o melhor do enxame,
        // atualiza a melhor partícula encontrada
        if(particle->getFitness() <= G_best->getFitness()){
            G_best->setPos(particle->getPos());
            G_best->updateFitness(particle->getFitness());
        }

        particles.push_back(particle);
    }
}

void Pso::updateFitness(const int particle_idx, const int topology_id){
    long double fitness = ackley(particles[particle_idx]->getPos(), ndimensions);
    particles[particle_idx]->updateFitness(fitness);
    
    if (fitness < G_best->getFitness()) {
        found_min = current_generation;
        G_best->setPos(particles[particle_idx]->getPos());
        G_best->setBestPos(particles[particle_idx]->getPos());
        G_best->updateFitness(fitness);
    }

    if(topology_id == 2){
        int num_columns = sqrt(npop);

        array<int, 5> neighbour_idx = {particle_idx - 1, particle_idx,
                                       particle_idx + 1, particle_idx - num_columns,
                                       particle_idx + num_columns};

        for (auto &idx : neighbour_idx){
            if (idx < 0 or idx >= npop) continue;

            if (fitness < particles[idx]->getPersonalGbestFitness()) {
                particles[idx]->setPersonalGbestFitness(fitness);
                particles[idx]->setPersonalGbestPos(particles[particle_idx]->getPos());
            }
        }
    }
}

void Pso::updateParticles(const int topology_id){
    for(int p = 0; p < npop; ++p){
        const vector<long double> &particle_best_pos = particles[p]->getBestPos();
        const vector<long double> &particle_pos = particles[p]->getPos();
        const vector<long double> &particle_velocity = particles[p]->getVelocity();
        const vector<long double> &g_best_pos = topology_id == 1 ? 
                                  G_best->getBestPos() : particles[p]->getPersonalGbestPos();
        

        vector<long double> new_velocity(ndimensions);
        for(int d = 0; d < ndimensions; ++d) {
            uniform_real_distribution<long double> r_gen(0.0f, 1.0f);
            long double r1 = r_gen(globalGenerator);
            long double r2 = r_gen(globalGenerator);

            long double cognitive_component = c1 * r1 * (particle_best_pos[d] - 
                                                         particle_pos[d]);
            long double social_component = c2 * r2 * (g_best_pos[d] - 
                                                      particle_pos[d]);

            new_velocity[d] = w * particle_velocity[d] + cognitive_component +
                              social_component;
           
            // Mantém a velocidade dentro dos limites estabelecidos
            new_velocity[d] = min(new_velocity[d], MAX_VELOCITY);
            new_velocity[d] = max(new_velocity[d], MIN_VELOCITY);
        }

        vector<long double> new_pos = particle_pos;
        for(int d = 0; d < ndimensions; ++d){
            new_pos[d] += new_velocity[d];
          
            // Mantém a posição dentro dos limites de busca
            new_pos[d] = min(new_pos[d], MAX_X);
            new_pos[d] = max(new_pos[d], MIN_X);
        }
       
        particles[p]->setPos(new_pos);

        updateFitness(p, topology_id);
    }
}

long double Pso::ackley(const vector<long double> &x, const int n){
    long double sum_1 = 0.0;
    long double sum_2 = 0.0;

    for(int i = 0; i < n; ++i){
        sum_1 += pow(x[i], 2);
        sum_2 += cos(2.0 * M_PI * x[i]);
    }

    return -20.0 * exp(-0.2 * sqrt((long double) 1.0/n * sum_1))
           - exp((long double) 1.0/n * sum_2) + 20.0 + exp(1.0);
}

int Pso::getNgen(void) const{
    return ngen;
}

int Pso::getNdimensions(void) const{
    return ndimensions;
}

int Pso::getNpop(void) const{
    return npop;
}

long double Pso::getC1(void) const{
    return c1;
}

long double Pso::getC2(void) const{
    return c2;
}

long double Pso::getW(void) const{
    return w;
}

long double Pso::getGbestFitness(void) const{
    return G_best->getFitness();
}

long double Pso::getAverageFitness(void) const{
    long double fitness_sum = 0.0f;
    for(auto particle : particles){
        fitness_sum += particle->getFitness();
    }
    return fitness_sum / npop;
}

void Pso::printBestPos(void) const{
    cout << fixed << setprecision(16);

    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "    Algoritmo de Otimizacao por Enxame de Particulas     " << endl;
    cout << "            para otimizar a funcao de Ackley             " << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "- Melhor resultado encontrado: " << G_best->getFitness() << endl;
    cout << "- Posicao: (";
    stringstream coordinates;
    for(const auto &x : G_best->getPos())
        coordinates << fixed << setprecision(16) << x << ", ";
    string s = coordinates.str();
    s.pop_back();
    s.pop_back();
    cout << s << ")" << endl;
    cout << "- Encontrado na geracao: " << found_min << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
}

Pso::Pso(const int _npop, const int _ngen, const int _ndimensions, 
         const long double _c1, const long double _c2, const long double _w) : 
    ngen(_ngen), ndimensions(_ndimensions), c1(_c1), c2(_c2), 
    w(_w), found_min(0), current_generation(0){
        int aux_npop = ceil(sqrt(_npop));
        npop = aux_npop * aux_npop;
        
        G_best = new Particle(ndimensions);

        vector<long double> pos;
        for(int i = 0; i < ndimensions; ++i) 
            pos.push_back(MAX_X);
        G_best->setPos(pos);
        G_best->updateFitness(ackley(pos, ndimensions));
    }

Pso::~Pso(){
    for(int p = 0; p < npop; ++p)
        delete particles[p];
    delete G_best;
}