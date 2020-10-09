// Copyright (C) 2020 Florian Krellner
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
#include <iostream>

namespace domain_propagation {

using namespace std;

template <typename I, typename V>
inline void domain_propagation( //
    const vector<I> &source,    //
    const vector<I> &target,    //
    vector<V> &capacity,        //
    vector<V> &supply,          //
    vector<V> &fixed_flow       //
) {
    I nVertices = supply.size();
    I nEdges = source.size();

    V capacity_reduction = 0;

    vector<V> activity_min(nVertices, 0);
    vector<V> activity_max(nVertices, 0);

    for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
        I v = source[eIdx];
        I w = target[eIdx];
        activity_max[v] += capacity[eIdx];
        activity_min[w] -= capacity[eIdx];
    }

    bool propagation_found = true;
    while (propagation_found) {

        propagation_found = false;
        for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
            I v = source[eIdx];
            I w = target[eIdx];

            V lb = max(supply[v] - activity_max[v] + capacity[eIdx],
                       -(supply[w] - activity_min[w]) + capacity[eIdx]);
            V ub = min(supply[v] - activity_min[v], -(supply[w] - activity_max[w]));

            if (ub <= capacity[eIdx] - 1) {
                V delta = capacity[eIdx] - ub;
                // update graph
                capacity[eIdx] -= delta;
                // update internal data
                activity_max[v] -= delta;
                activity_min[w] += delta;
                propagation_found = true;
                // needed for logging
                capacity_reduction += delta;
            }

            if (lb >= 1) {
                // update graph
                capacity[eIdx] -= lb;
                fixed_flow[eIdx] += lb;
                supply[v] -= lb;
                supply[w] += lb;
                // update internal data
                propagation_found = true;
                // needed for logging
                capacity_reduction += lb;
            }
        }
    }

    I ctr = 0;
    for (I eIdx = 0; eIdx < nEdges; ++eIdx)
        if (capacity[eIdx] < 0.5)
            ctr++;

    std::cout << "edge reduction by " << ctr << std::endl;
}

} // namespace domain_propagation
