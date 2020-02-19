#pragma once

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

#include <vector>
#include <numeric> 

using namespace std;

//
//
//
//
template <typename I,
          typename V>
inline void initialize(
    const I nEdges,                           // number of edges in the original graph
    const I nVertices,                        // number of vertices in the original graph
    vector<V> &capacity,                      //
    vector<V> &flow,                          //
    vector<V> &supply,                        //
    vector<V> &costs,                         //
    vector<V> &potentials,                    //
    vector<I> &number_successors,             //
    vector<I> &last_successor,                //
    vector<I> &parent,                        //
    vector<I> &predecessor,                   //
    vector<I> &source,                        //
    vector<I> &target,                        //
    vector<I> &thread,                        //
    vector<I> &reversed_thread,               //
    vector<Direction> &direction_predecessor, //
    vector<State> &state                      //
)
{
    iota(last_successor.begin(), last_successor.end(), I(0));
    iota(predecessor.begin(), predecessor.end(), nEdges);
    iota(thread.begin(), thread.end(), 1);
    iota(++reversed_thread.begin(), reversed_thread.end(), I(0));

    flow.resize(nEdges, 0);
    flow.reserve(nEdges + nVertices);
    source.reserve(nEdges + nVertices);
    target.reserve(nEdges + nVertices);
    costs.reserve(nEdges + nVertices);
    direction_predecessor.reserve(nEdges + nVertices);

    V sum_costs = accumulate(costs.begin(), costs.end(),V(1));

    // V sum_costs = V(0.9*numeric_limits<int>::max());

    I root = nVertices;

    for (I u = 0; u < root; ++u)
    {
        if (supply[u] >= 0)
        {
            direction_predecessor[u] = UP;
            source.push_back(u);
            target.push_back(root);
            flow.push_back(supply[u]);
            costs.push_back(0);
        }
        else
        {
            direction_predecessor[u] = DOWN;
            source.push_back(root);
            target.push_back(u);
            flow.push_back(-supply[u]);
            costs.push_back(sum_costs);
            potentials[u] = sum_costs;
        }
    }

    state.resize(nEdges + nVertices, TREE);
    // capacity.resize(nEdges + nVertices, numeric_limits<V>::max());
    capacity.resize(nEdges + nVertices, numeric_limits<V>::max());

    // initialize tree structure for root (vertex with index nVertices)
    parent[root] = root + 1;
    predecessor[root] = nEdges + nVertices;
    potentials[root] = 0;
    last_successor[root] = root - 1;
    number_successors[root] = nVertices;
    thread[root] = 0;
    reversed_thread[0] = root;
}

template <typename I,
          typename V,
          typename Logger>
inline void initialize(
    const I nEdges,                           // number of edges in the original graph
    const I nVertices,                        // number of vertices in the original graph
    vector<V> &capacity,                      //
    vector<V> &flow,                          //
    vector<V> &supply,                        //
    vector<V> &costs,                         //
    vector<V> &potentials,                    //
    vector<I> &number_successors,             //
    vector<I> &last_successor,                //
    vector<I> &parent,                        //
    vector<I> &predecessor,                   //
    vector<I> &source,                        //
    vector<I> &target,                        //
    vector<I> &thread,                        //
    vector<I> &reversed_thread,               //
    vector<Direction> &direction_predecessor, //
    vector<State> &state,                     //
    Logger &logger                            //
)
{
    logger.start();
    initialize<I,V>(
        nEdges, nVertices, capacity, flow, supply, costs, potentials, number_successors, last_successor,
        parent, predecessor, source, target, thread, reversed_thread, direction_predecessor, state);
    logger.end();
    logger.increment_initialize();
}