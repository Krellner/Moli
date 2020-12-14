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

#include <limits>
#include <vector>

#include <cassert>
#include <array>

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

namespace primal_network_simplex {

using namespace std;

// modulo for assumption a < 2*q
template <typename Z> inline void simple_modulo(Z &a, const Z q) {
    if (a >= q)
        a -= q;
}

// modulo for assumption a < q
template <typename Z> inline void no_modulo(Z &a, const Z q) {}

template <typename I, typename V, void modulo(I &, const I)>
inline void get_minimum_reduced_costs_in_block( //
    const I begin,                              //
    const I end,                                //
    const I nEdges,                             //
    const vector<V> &costs,                     //
    const vector<V> &potentials,                //
    const vector<I> &source,                    //
    const vector<I> &target,                    //
    const vector<State> &state,                 //
    I &arg_min                                  //
) {
    V min = 0;
    for (I eIdx = begin; eIdx != end; modulo(++eIdx, nEdges)) {
        V reduced_costs =
            V(state[eIdx]) * (costs[eIdx] + potentials[source[eIdx]] - potentials[target[eIdx]]);

        if (reduced_costs < min) {
            min = reduced_costs;
            arg_min = eIdx;
        }
    }
}

template <typename I,
          typename V>
inline void block_search(        //
    const I block_size,          //
    const I nEdges,              //
    const vector<V> &costs,      //
    const vector<V> &potentials, //
    const vector<I> &source,     //
    const vector<I> &target,     //
    const vector<State> &state,  //
    I &entering_edge,            //
    I &start_edge                //
) {
    // The block search pricing procedure cyclically computes the reduced costs blockwise and
    // selects the edge with the highest reduced costs among them. If the reduced costs are
    // negative, the pricing step stops.
    I nBlocks = nEdges / block_size + 1;

    I begin = start_edge;
    I end = start_edge + block_size; // < nEdges
    for (I block = 0; block < nBlocks; ++block) {
        simple_modulo(end, nEdges);

        if (begin < end)
            get_minimum_reduced_costs_in_block<I, V, no_modulo>( //
                begin, end, nEdges, costs, potentials, source, target, state, entering_edge);
        else
            get_minimum_reduced_costs_in_block<I, V, simple_modulo>( //
                begin, end, nEdges, costs, potentials, source, target, state, entering_edge);

        if (entering_edge < nEdges) {
            start_edge = start_edge + (block + 1) * block_size;
            simple_modulo(start_edge, nEdges);
            return;
        }

        begin = end;
        end += block_size;
    }
}

template <typename I, typename V,
          typename Logger>
inline void block_search(        //
    const I block_size,          //
    const I nEdges,              //
    const vector<V> &costs,      //
    const vector<V> &potentials, //
    const vector<I> &source,     //
    const vector<I> &target,     //
    const vector<State> &state,  //
    I &entering_edge,            //
    I &start_edge,               //
    Logger &logger               //
) {
    logger.start();

    block_search<I, V>(block_size, nEdges, costs, potentials, source, target, state, entering_edge,
                       start_edge);

    logger.end();
    logger.increment_find_entering();
}

} // namespace primal_network_simplex
