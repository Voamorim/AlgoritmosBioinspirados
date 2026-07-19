#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <bits/stdc++.h>

using namespace std;

class Particle{
    private:
        vector<long double> pos;
        vector<long double> best_pos;
        vector<long double> velocity;
        vector<long double> G_best_pos;
        long double fitness;
        long double best_fitness;
        long double G_best_fitness;
    public:
        const vector<long double>& getVelocity(void) const;
        void setVelocity(const vector<long double> &new_velocity);

        const vector<long double>& getPos(void) const;
        void setPos(const vector<long double> &new_pos);

        const vector<long double>& getBestPos(void) const;
        void setBestPos(const vector<long double> &new_best_pos);

        const long double getFitness(void) const;
        void updateFitness(const long double new_fitness);

        const long double getBestFitness(void) const;

        const vector<long double>& getPersonalGbestPos(void) const;
        const long double getPersonalGbestFitness(void) const;
        void setPersonalGbestPos(const vector<long double> &new_personal_g_best_pos);
        void setPersonalGbestFitness(const long double new_personal_g_best_fit);


        Particle(const int ndimensions);
        Particle(void);
        ~Particle();
};

#endif