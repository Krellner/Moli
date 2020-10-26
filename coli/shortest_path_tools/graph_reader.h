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

#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

template <typename I,
          typename V>
void read_graph_instance( //
    char *file_name,      //
    I &nVertices,         //
    vector<I> &source,    //
    vector<I> &target,    //
    vector<V> &costs      //
) {
    ifstream graph_file;
    graph_file.open(file_name);
    string line;
    I eIdx = 0;
    bool first_line = true;
    while (getline(graph_file, line)) {
        istringstream iss(line);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};
        if (first_line) {
            nVertices = stoi(tokens[0]);
            source.resize(stoi(tokens[1]));
            target.resize(stoi(tokens[1]));
            costs.resize(stoi(tokens[1]));
            first_line = false;
        } else {
            source[eIdx] = stoi(tokens[0]) - 1;
            target[eIdx] = stoi(tokens[1]) - 1;
            costs[eIdx] = stoi(tokens[2]);
            eIdx++;
        }
    }
}
