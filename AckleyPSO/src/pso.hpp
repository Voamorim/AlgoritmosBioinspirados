#ifndef PSO_HPP
#define PSO_HPP

#include "particle.hpp"

#include <bits/stdc++.h>

using namespace std;

class Pso{
    private:
        int npop; // Tamano da população
        int ngen; // Número máximo de iterações
        int ndimensions; // Número de dimensões

        long double c1; // Taxa de aprendizado cognitiva
        long double c2; // Taxa de aprendizado social
        long double w; // Ponderação de inércia (fator de diversificação)

        Particle *G_best;

        const long double MAX_X = 2.0;
        const long double MIN_X = -MAX_X;

        const long double MAX_VELOCITY = 0.05;
        const long double MIN_VELOCITY = -MAX_VELOCITY;

        // Função objetivo
        long double ackley(const vector<long double> &x, const int n);
    public: 
        // Vetor para armazenar as particulas do enxame
        vector<Particle*> particles;

        int found_min; 
        int current_generation;

        void createInitialPopulation(void);
        void updateFitness(const int particle_idx, const int topology_id);
        void updateParticles(const int topology_id);

        int getNgen(void) const;
        int getNpop(void) const;
        int getNdimensions(void) const;
        long double getC1(void) const;
        long double getC2(void) const;
        long double getW(void) const;
        long double getGbestFitness(void) const;
        long double getAverageFitness(void) const;

        void printBestPos(void) const;

        Pso(const int _npop, const int _ngen, const int _ndimensions, 
            const long double _c1, const long double _c2, const long double _w);
        ~Pso();
};

#endif