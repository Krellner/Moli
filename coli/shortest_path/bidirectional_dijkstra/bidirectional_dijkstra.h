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
#include <numeric>

#include "../../../tools/argsort/argsort.h"
#include "bidirectional_dijkstra_relax.h"

namespace bidirectional_dijkstra {

using namespace std;

//
//
//
//
template <typename I, typename V>
V bidirectional_dijkstra(             //
    const I origin,                   //
    const I desitination,             //
    const vector<I> &forward_star,    //
    const vector<I> &forward_source,  //
    const vector<I> &forward_target,  //
    const vector<V> &forward_costs,   //
    const vector<I> &backward_star,   //
    const vector<I> &backward_source, //
    const vector<I> &backward_target, //
    const vector<V> &backward_costs,  //
    I &connector,                     //
    vector<I> &forward_parent,        //
    vector<I> &backward_parent        //
) {
    I nVertices = forward_star.size();
    I nEdges = forward_source.size();

    // argsort target, todo: own function

    // vector<I> arg_sort_target(nEdges);
    // iota(arg_sort_target.begin(), arg_sort_target.end(), 0);
    // sort(arg_sort_target.begin(), arg_sort_target.end(),
    //      [&](int i, int j) -> bool { return target[i] < target[j]; });

    // create reverse graph in star-graph like datastructure
    // vector<I> reversed_source(target);
    // sort(reversed_source.begin(), reversed_source.end());
    // vector<I> reversed_target;
    // vector<V> reversed_costs;
    // reversed_target.reserve(nEdges);
    // reversed_costs.reserve(nEdges);
    // for (I i : arg_sort_target) {
    //     reversed_target.push_back(source[i]);
    //     reversed_costs.push_back(costs[i]);
    // }

    // see (1); todo: own function
    // vector<I> reversed_start(nVertices + 1, nEdges);
    // for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
    //     I v = reversed_source[eIdx];
    //     reversed_start[v] = min(start[v], eIdx);
    // }

    // ------------------------------------------------
    // data structure for bidirection Dijkstra

    minimum_priority_queue<V, I> forward_queue;
    minimum_priority_queue<V, I> backward_queue;
    forward_queue.push(make_pair(0, origin));
    backward_queue.push(make_pair(0, desitination));

    V sum_costs = accumulate(forward_costs.begin(), forward_costs.end(), I(0));
    vector<V> forward_distance(nVertices, sum_costs + 1);
    vector<V> backward_distance(nVertices, sum_costs + 1);
    forward_distance[origin] = 0;
    backward_distance[desitination] = 0;

    // ------------------------------------------------
    // Dijsktra loop

    V mu = sum_costs;
    while (forward_queue.top().first + backward_queue.top().first < mu) {
        // forward dijsktra from origin
        I v = forward_queue.top().second;
        forward_queue.pop();
        relax(v, forward_star, forward_source, forward_target, forward_costs, backward_distance,
              connector, mu, forward_parent, forward_distance, forward_queue);
        // backward dijkstra from destination
        I w = backward_queue.top().second;
        // std::cout << "(" << backward_queue.top().first << " , " << backward_queue.top().second
        //           << ")" << std::endl;
        backward_queue.pop();
        // std::cout << "(" << backward_queue.top().first << " , " << backward_queue.top().second
        //           << ")" << std::endl;
        relax(w, backward_star, backward_source, backward_target, backward_costs, forward_distance,
              connector, mu, backward_parent, backward_distance, backward_queue);
    }

    return mu;
}
} // namespace bidirectional_dijkstra