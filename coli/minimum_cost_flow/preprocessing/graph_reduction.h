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

namespace graph_reduction {

using namespace std;

template <typename I, typename V>
inline void graph_reduction( //
    vector<I> &source,       //
    vector<I> &target,       //
    vector<I> &indices,      //
    vector<V> &costs,        //
    vector<V> &capacity      //
) {
    I nEdges = source.size();

    vector<I> transformed_source;
    vector<I> transformed_target;
    vector<V> transformed_costs;
    vector<V> transformed_capacities;

    transformed_source.reserve(nEdges);
    transformed_target.reserve(nEdges);
    transformed_costs.reserve(nEdges);
    transformed_capacities.reserve(nEdges);

    for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
        if (capacity[eIdx] >= 1) {
            indices.push_back(eIdx);
            transformed_source.push_back(source[eIdx]);
            transformed_target.push_back(target[eIdx]);
            transformed_costs.push_back(costs[eIdx]);
            transformed_capacities.push_back(capacity[eIdx]);
        }
    }

    source = transformed_source;
    target = transformed_target;
    costs = transformed_costs;
    capacity = transformed_capacities;
}

} // namespace graph_reduction