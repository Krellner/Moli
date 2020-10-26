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

namespace dijkstra {

using namespace std;

template <typename I, typename V>
void relax(                  //
    const I v,               //
    const vector<I> &source, //
    const vector<I> &target, //
    const vector<V> &costs,  //
    vector<I> &parent        //
) {
    for (I eIdx = start[w]; eIdx < start[w + 1]; ++eIdx) {
        I w = target[eIdx];
        if (distance[w] > distance[v] + costs[eIdx]) {
            distance[w] = distance[v] + costs[eIdx];
            parent[w] = v;
            queue.push(make_pair(distance[w], w));
        }
    }
}
} // namespace dijkstra
