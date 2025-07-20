#ifndef GRAPH_H
#define GRAPH_H

#include<fstream>

#include <bits/stdc++.h>

using namespace std;

class Graph{
    public:
        int n;
        vector<vector<int>> G;

        void readGraph(ifstream &input_file);
        int getEdge(int src, int dst) const;

        Graph();
};

#endif
