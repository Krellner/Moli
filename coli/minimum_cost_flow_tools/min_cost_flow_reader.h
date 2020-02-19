#pragma once

#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

typedef std::pair<size_t, size_t> edge;

void readMinimumCostFlowInstance(
    char *file_name,      //
    vector<edge> &edges,  //
    vector<long int> &costs, //
    vector<long int> &ub,      //
    vector<long int> &supply   //
)
{
    ifstream graph_file;
    graph_file.open(file_name);
    string line;
    size_t edgeId = 0;
    while (getline(graph_file, line))
    {
        // extract information from the line
        istringstream iss(line);
        vector<string> tokens{
            istream_iterator<string>{iss},
            istream_iterator<string>{}};

        if (tokens.size() > 0)
        {
            if (tokens[0] == "p")
            {
                // tokens[2] = number of vertices; tokens[3] = number of edges
                supply.resize(stoi(tokens[2]));
                edges.resize(stoi(tokens[3]));
                costs.resize(stoi(tokens[3]));
                ub.resize(stoi(tokens[3]));
            }

            if (tokens[0] == "n")
                supply.at(stoi(tokens[1]) - 1) = stoi(tokens[2]);

            if (tokens[0] == "a")
            {
                edges.at(edgeId).first = stoi(tokens[1]) - 1;
                edges.at(edgeId).second = stoi(tokens[2]) - 1;
                ub.at(edgeId) = stoi(tokens[4]);
                costs.at(edgeId) = stoi(tokens[5]);
                edgeId++;
            }
        }
    }
}