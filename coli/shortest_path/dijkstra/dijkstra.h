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

#include <queue>

namespace dijkstra {

using namespace std;

//
//
//
//
template <typename I, typename V, typename Logger>
void dijkstra(         //
    const I nVertices, //
    const I origin,    //
    vector<I> &source, //
    vector<I> &target, //
    vector<V> &costs,  //
    vector<I> &path,   //
    Logger &logger     //
) {
    I nEdges = source.size();

    vector<I> start(nVertices + 1, nEdges);

    for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
        I v = source[eIdx];
        start[v] = min(start[v], eIdx);
    }

    priority_queue<pair<V, I>> queue;

    queue.push_back(make_pair(0, origin));

    while (!queue.empty()) {
        
    }
}

} // namespace dijkstra