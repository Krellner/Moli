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

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"
#include "primal_network_simplex_block_search.h"
#include "primal_network_simplex_leaving_edge.h"

#include <vector>

using namespace std;

// Pivot
//  performes the pivot step, that is:
//  1. find entering edge,
//  2. find leaving edge and the first comman predecessor of the two verices of the entering edge (called apex)
template <typename I,
          typename V,
          typename Logger,
          PivotRule Rule>
void pivot(
    const I block_size,                             // size of the blocks that are searched to find the entering edge
    const I nEdges,                                 // number of edges in the original graph
    const vector<V> &capacity,                      //
    const vector<V> &flow,                          //
    const vector<V> &supply,                        //
    const vector<V> &costs,                         //
    const vector<V> &potentials,                    //
    const vector<I> &number_successors,             //
    const vector<I> &parent,                        //
    const vector<I> &predecessor,                   //
    const vector<I> &source,                        //
    const vector<I> &target,                        //
    const vector<Direction> &direction_predecessor, //
    const vector<State> &state,                     //
    Change &change,                                 //
    V &delta,                                       //
    I &apex,                                        // apex of v and w, whereas (v,w) is the entering edge
    I &length,                                      //
    I &entering_edge,                               // index of the entering edge, initialized with nEdges
    I &in_v,                                        //
    I &in_w,                                        //
    I &out_v,                                       //
    I &leaving_edge,                                // size of path from leaving to entering edge
    I &start_edge,                                  // index of the edge at which searching for there entering edge starts
    Logger &logger                                  //
)
{
    // 1. find entering edge
    blockSearch<I, V, Logger>(
        block_size, nEdges, costs, potentials, source, target, state, entering_edge, start_edge, logger);
    
    // if no edge was found, terminate
    if (entering_edge == nEdges) 
        return;

    // 2. find leaving edge
    I v = source[entering_edge], w = target[entering_edge]; // flow is pushed from first to second
    if (state[entering_edge] == UPPER)                      // if at upper bound the flow must be reduced, flow from target to source
        swap(v, w);
    delta = capacity[entering_edge];
    findLeavingEdge<I, V, Logger>(
        v, w, capacity, flow, predecessor, parent, number_successors, direction_predecessor, change,
        delta, apex, leaving_edge, length, in_v, in_w, out_v, logger);
}