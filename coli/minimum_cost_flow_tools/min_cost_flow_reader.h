// Copyright (C) 2019-2020 Florian Krellner
//
// Permission to use, modify and distribute this software is granted
// provided that this copyright notice appears in all copies. For
// precise terms see the accompanying LICENSE file.
//
// This software is provided "AS IS" with no warranty of any kind,
// express or implied, and with no claim as to its suitability for any
// purpose.

#pragma once

#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

typedef std::pair<size_t, size_t> edge;

template <typename I,
          typename V>
void readMinimumCostFlowInstance(
    char *file_name,      //
    vector<I> &source,  //
    vector<I> &target,  //
    vector<V> &costs, //
    vector<V> &ub,      //
    vector<V> &supply   //
)
{
    ifstream graph_file;
    graph_file.open(file_name);
    string line;
    I edgeIdx = 0;
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
                source.resize(stoi(tokens[3]));
                target.resize(stoi(tokens[3]));
                costs.resize(stoi(tokens[3]));
                ub.resize(stoi(tokens[3]));
            }

            if (tokens[0] == "n")
                supply[stoi(tokens[1]) - 1] = stoi(tokens[2]);

            if (tokens[0] == "a")
            {
                source[edgeIdx] = stoi(tokens[1]) - 1;
                target[edgeIdx] = stoi(tokens[2]) - 1;
                ub[edgeIdx] = stoi(tokens[4]);
                costs[edgeIdx] = stoi(tokens[5]);
                edgeIdx++;
            }
        }
    }
}