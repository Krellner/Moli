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

using namespace std;

template <typename I, typename V>
inline void domain_propagation( //
    const vector<I> &source,    //
    const vector<I> &target,    //
    vector<V> &capacity,        //
    vector<V> &supply           //
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

    vector<V> lb(nEdges, 0);
    vector<V> ub(capacity);

    bool propagation_found = true;

    while (propagation_found) {
        propagation_found = false;
        for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
            I v = source[eIdx];
            I w = target[eIdx];

            V lb_1 = supply[v] - activity_max[v] + ub[eIdx];
            V lb_2 = -(supply[w] - activity_min[w]) + ub[eIdx];

            V ub_1 = supply[v] - activity_min[v] + lb[eIdx];
            V ub_2 = -(supply[w] - activity_max[w]) + lb[eIdx];

            if (ub_1 <= ub[eIdx] - 1) {
                V delta = ub[eIdx] - ub_1;
                ub[eIdx] -= delta;
                activity_max[v] -= delta;
                activity_min[w] += delta;
                propagation_found = true;
                capacity_reduction += delta;
            }
            if (ub_2 <= ub[eIdx] - 1) {
                V delta = ub[eIdx] - ub_2;
                ub[eIdx] -= delta;
                activity_max[v] -= delta;
                activity_min[w] += delta;
                propagation_found = true;
                capacity_reduction += delta;
            }

            if (lb_1 >= lb[eIdx] + 1) {
                V delta = lb_1 - lb[eIdx];
                lb[eIdx] += delta;
                activity_max[v] += delta;
                activity_min[w] -= delta;
                propagation_found = true;
                capacity_reduction += delta;
            }
            if (lb_2 >= lb[eIdx] + 1) {
                V delta = lb_2 - lb[eIdx];
                lb[eIdx] += delta;
                activity_max[v] += delta;
                activity_min[w] -= delta;
                propagation_found = true;
                capacity_reduction += delta;
            }
        }
    }

    // capacity = ub - lb
    // std::transform(
    //     being(capacity), end(capacity),
    //     [ub_i = begin(ub), lb_i = begin(lb)] mutable(auto &) { return *ub_i++ - *lb_i++; }
    // );

    for (I eIdx = 0; eIdx < nEdges; ++eIdx)
        capacity[eIdx] = ub[eIdx] - lb[eIdx];

    for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
        if (lb[eIdx] > 0) {
            supply[source[eIdx]] -= lb[eIdx];
            supply[target[eIdx]] += lb[eIdx];
        }
    }

    std::cout << "capacity reduction by " << capacity_reduction << std::endl;

    for (I eIdx = 0; eIdx < nEdges; ++eIdx) 
        if(capacity[eIdx] < 0.5)
            std::cout << eIdx << " with 0 capacity " << std::endl;

}
