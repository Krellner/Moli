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

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"
#include "primal_network_simplex_pivot.h"
#include "primal_network_simplex_update.h"
#include "primal_network_simplex_initialize.h"

using namespace std;

//
//
//
//
template <typename I, typename V, typename Logger,
          PivotRule Rule>
void primal_network_simplex(  //
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

    // Primal Network Simplex Algorithm runs on auxiliary graph. The auxiliary graph has one more
    // vertex than the given graph. Any vertex is connect to this auxiliary vertex. Those edges
    // have high costs, so they are not part of an optimal solution. On this auxiliary graph,
    // finding an initial solution is trivial. See the initialization step for more details.

    supply.push_back(0);

    vector<V> potentials(nVertices + 1, 0);
    vector<I> parent(nVertices + 1, nVertices);
    vector<I> predecessor(nVertices + 1, nEdges + nVertices);
    vector<I> thread(nVertices + 1);
    vector<I> reversed_thread(nVertices + 1, 0);
    vector<I> number_successors(nVertices + 1, 1);
    vector<I> last_successor(nVertices + 1);
    vector<Direction> direction_predecessor(nVertices + 1);
    vector<State> state(nEdges, LOWER);

    initialize<I, V, Logger>(nEdges, nVertices, capacity, flow, supply, costs, potentials,
                             number_successors, last_successor, parent, predecessor, source, target,
                             thread, reversed_thread, direction_predecessor, state, logger);

    // Block size for the pricing step, this is a good value found by experimenting.
    const I block_size = I(sqrt(I(nEdges)));
    // Start_edge is the edge at which to start the pricing.
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