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

#include <math.h>

#include <typeinfo>
#include <vector>

#include "primal_network_simplex_initialize.h"
#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"
#include "primal_network_simplex_pivot.h"
#include "primal_network_simplex_update.h"

using namespace std;

//
//
//
//
template <typename I, typename V, typename Logger,
          PivotRule Rule>
void primalNetworkSimplex(  //
    vector<I>& source,      //
    vector<I>& target,      //
    vector<V>& costs,       //
    vector<V>& capacity,    //
    vector<V>& supply,      //
    vector<V>& flow,        //
    Logger& logger          //
)
{
    const I nVertices = supply.size();
    const I nEdges = target.size();

    supply.push_back(0);
    vector<V> potentials(nVertices + 1, 0);

    // data structures for storing the spanning tree representation

    vector<I> parent(nVertices + 1, nVertices);  // for each vertex the parent vertex
    vector<I> predecessor(nVertices + 1,
                          nEdges + nVertices);  // for each vertex the incoming tree edge
    vector<I> thread(nVertices + 1);  // for each vertex ... (used for updating the potentials)
    vector<I> reversed_thread(nVertices + 1, 0);    // for each vertex ... (only used for updating)
    vector<I> number_successors(nVertices + 1, 1);  // for each vertex the number of successors
    vector<I> last_successor(nVertices + 1);        // for each vertex its last successors
    vector<Direction> direction_predecessor(
        nVertices + 1);                  // for each vertex the orientation of its predecessor
    vector<State> state(nEdges, LOWER);  // for each edge it status with in the spanning tree

    initialize<I, V, Logger>(nEdges, nVertices, capacity, flow, supply, costs, potentials,
                             number_successors, last_successor, parent, predecessor, source, target,
                             thread, reversed_thread, direction_predecessor, state, logger);

    const I block_size = I(sqrt(I(nEdges)));

    I start_edge = 0;
    while (true)
    {
        Change change;
        V delta;
        I apex;
        I entering_edge = nEdges;
        I leaving_edge = nEdges + nVertices;
        I length;
        I in_v, in_w, out_v;

        pivot<I, V, Logger, Rule>(
            block_size, nEdges, capacity, flow, supply, costs, potentials, number_successors,
            parent, predecessor, source, target, direction_predecessor, state, change, delta, apex,
            length, entering_edge, in_v, in_w, out_v, leaving_edge, start_edge, logger);

        if (entering_edge == nEdges) break;

        update<I, V, Logger>(change, delta, apex, entering_edge, source[entering_edge],
                             target[entering_edge], leaving_edge, length, in_v, in_w, out_v, costs,
                             potentials, flow, last_successor, number_successors, parent,
                             predecessor, reversed_thread, thread, direction_predecessor, state,
                             logger);
    }
}