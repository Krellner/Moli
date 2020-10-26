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
#include <queue>

namespace bidirectional_dijkstra {

using namespace std;

template <typename I, typename V>
using minimum_priority_queue = priority_queue<pair<V, I>, vector<pair<V, I>>, greater<pair<V, I>>>;

//
//
//
//
template <typename I, typename V>
void relax(                             //
    const I v,                          //
    const vector<I> &star,              //
    const vector<I> &source,            //
    const vector<I> &target,            //
    const vector<V> &costs,             //
    const vector<V> &estimates,         //
    I &connector,                       //
    V &estimate,                        //
    vector<I> &parent,                  //
    vector<V> &distance,                //
    minimum_priority_queue<V, I> &queue //
) {
    for (I eIdx = star[v]; eIdx < star[v + 1]; ++eIdx) {
        I w = target[eIdx];
        if (distance[w] > distance[v] + costs[eIdx]) {
            distance[w] = distance[v] + costs[eIdx];
            parent[w] = v;
            queue.push(make_pair(distance[w], w));
            if (estimate > distance[w] + estimates[w]) {
                estimate = distance[w] + estimates[w];
                connector = w;
            }
        }
    }
}
} // namespace bidirectional_dijkstra
