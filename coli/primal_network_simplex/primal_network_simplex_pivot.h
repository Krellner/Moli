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

#include <vector>

#include "primal_network_simplex_block_search.h"
#include "primal_network_simplex_leaving_edge.h"
#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

using namespace std;

template <typename I, typename V, typename Logger, PivotRule Rule>
void pivot(                                           //
    const I block_size,                               //
    const I nEdges,                                   //
    const vector<V> &capacity,                        //
    const vector<V> &flow,                            //
    const vector<V> &supply, const vector<V> &costs,  //
    const vector<V> &potentials,                      //
    const vector<I> &number_successors,               //
    const vector<I> &parent,                          //
    const vector<I> &predecessor,                     //
    const vector<I> &source,                          //
    const vector<I> &target,                          //
    const vector<Direction> &direction_predecessor,   //
    const vector<State> &state,                       //
    Change &change,                                   //
    V &delta,                                         //
    I &apex,                                          //
    I &length,                                        //
    I &entering_edge,                                 //
    I &in_v,                                          //
    I &in_w,                                          //
    I &out_v,                                         //
    I &leaving_edge,                                  //
    I &start_edge,                                    //
    Logger &logger                                    //
)
{
    // 1. Find entering edge (pricing).
    // 2. Find leaving edge and the first common predecessor of the two vertices of the entering
    //    edge (called apex).

    blockSearch<I, V, Logger>(block_size, nEdges, costs, potentials, source, target, state,
                              entering_edge, start_edge, logger);

    if (entering_edge == nEdges) return;

    I v = source[entering_edge], w = target[entering_edge];
    if (state[entering_edge] == UPPER) swap(v, w);
    delta = capacity[entering_edge];

    findLeavingEdge<I, V, Logger>(v, w, capacity, flow, predecessor, parent, number_successors,
                                  direction_predecessor, change, delta, apex, leaving_edge, length,
                                  in_v, in_w, out_v, logger);
}