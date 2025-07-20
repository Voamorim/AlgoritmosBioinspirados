#include "graph.hpp"
#include <bits/stdc++.h>

Graph::Graph(){}

void Graph::readGraph(ifstream &input_file){
    string line;
    while(getline(input_file, line)){
        if(line.empty() or line[0] == '#') continue;
        istringstream line_stream(line);
        string str_cost;
        vector<int> costs;

        while(line_stream >> str_cost){
            int cost = stoi(str_cost);
            costs.push_back(cost);
        }

        G.push_back(costs);
    }
    n = G.size();
}

int Graph::getEdge(int src, int dst) const {
    return G[src][dst];
}
