#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <bits/stdc++.h>

using namespace std;

class Graph{
    private:
        int n;
        vector<vector<double>> G;

    public:
        void initGraph(const int n);
        void resetGraph(void);
        void readGraph(ifstream &input_file);
        double getEdge(const int src, const int dest) const;
        void setEdge(const int src, const int dest, const double cost);
        void incrementEdge(const int src, const int dest, const double increment);
        int getNumVertices() const;

        Graph();
        Graph(const int num_vertices);
};

#endif
