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

// #include "tbb/parallel_for.h"
// #include "tbb/blocked_range.h"
// #include "tbb/tbb.h"

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

using namespace std;

// assumes a < 2*q
template <typename V> inline V simple_modulo(const V a, const V q) { return (a < q) ? a : a - q; }

template <typename I, typename V>
inline void get_minimum_reduced_costs_in_block( //
    const I begin,                              //
    const I end,                                //
    const vector<V> &costs,                     //
    const vector<V> &potentials,                //
    const vector<I> &source,                    //
    const vector<I> &target,                    //
    const vector<State> &state,                 //
    V &min,                                     //
    I &arg_min                                  //
) {
    for (I eIdx = begin; eIdx < end; ++eIdx) {
        V reduced_costs =
            V(state[eIdx]) * (costs[eIdx] + potentials[source[eIdx]] - potentials[target[eIdx]]);
        if (reduced_costs < min) {
            min = reduced_costs;
            arg_min = eIdx;
        }
    }
}

template <typename I, typename V>
inline void get_minimum_reduced_costs_in_block( //
    const I begin,                              //
    const I end,                                //
    const I nEdges,                             //
    const vector<V> &costs,                     //
    const vector<V> &potentials,                //
    const vector<I> &source,                    //
    const vector<I> &target,                    //
    const vector<State> &state,                 //
    V &min,                                     //
    I &arg_min                                  //
) {
    for (I idx = begin; idx < end; ++idx) {
        I eIdx = simple_modulo(idx, nEdges);
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
    // selects the edge with the most reduced costs among them at each iteration. If an edge with
    // negative reduced costs is found in a block it is selected and the pricing stops. The next
    // round starts with the next block.
    //
    // The number of edges might not be devideable by the block size. So there will most likely be a
    // block that contains the last and the first edge. We separately consider the blocks before the
    // mentioned block, the mention block and the blocks after the mentioned block, in the stated
    // order.

    V min = 0;
    I nBlocks = nEdges / block_size + 1;

    for (I block = 0; block < nBlocks; ++block) {
        I eIdx;
        I begin = start_edge + block * block_size;
        I end = start_edge + (block + 1) * block_size;

        if (end < nEdges) {
            get_minimum_reduced_costs_in_block<I, V>(begin, end, costs, potentials, source, target,
                                                     state, min, entering_edge);
        } else if (begin > nEdges) {
            get_minimum_reduced_costs_in_block<I, V>(begin - nEdges, end - nEdges, costs,
                                                     potentials, source, target, state, min,
                                                     entering_edge);
        } else {
            get_minimum_reduced_costs_in_block<I, V>(begin, end, nEdges, costs, potentials, source,
                                                     target, state, min, entering_edge);
        }

        if (min < 0) {
            // this is end if start_edge didn't change
            start_edge = simple_modulo(start_edge + (block + 1) * block_size, nEdges);
            return;
        }
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
