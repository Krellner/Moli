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

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

using namespace std;

template <typename I, typename V>
inline void get_minimum_reduced_costs_in_block(  //
    const I begin,                               //
    const I end,                                 //
    const vector<V>& costs,                      //
    const vector<V>& potentials,                 //
    const vector<I>& source,                     //
    const vector<I>& target,                     //
    const vector<State>& state,                  //
    V& min,                                      //
    I& arg_min                                   //
)
{
    for (I eIdx = begin; eIdx < end; ++eIdx)
    {
        V reduced_costs =
            V(state[eIdx]) * (costs[eIdx] + potentials[source[eIdx]] - potentials[target[eIdx]]);
        if (reduced_costs < min)
        {
            min = reduced_costs;
            arg_min = eIdx;
        }
    }
}

template <typename I,
          typename V>
inline void blockSearch(          //
    const I block_size,           //
    const I nEdges,               //
    const vector<V>& costs,       //
    const vector<V>& potentials,  //
    const vector<I>& source,      //
    const vector<I>& target,      //
    const vector<State>& state,   //
    I& entering_edge,             //
    I& start_edge                 //
)
{
    // The block search pricing procedure cyclically computes the reduced costs blockwise and
    // selects the edge with the most reduced costs among them at each iteration. If an edge with
    // negitve reduced costs is found in a block it is selected and the pricing stops. The next
    // round starts with the next block.
    //
    // The number of edges might not be devideable by the block size. So there will most likely be a
    // block that contains the last and the first edge. We seperatly consider the blocks before the
    // mentioned block, the mention block and the blocks after the mentioned block, in the stated
    // order.

    V min = 0;

    I begin = start_edge;
    I end = start_edge + block_size;
    while (end <= nEdges)
    {
        get_minimum_reduced_costs_in_block<I, V>(begin, end, costs, potentials, source, target,
                                                 state, min, entering_edge);
        if (min < 0)
        {
            start_edge = end;
            return;
        }
        begin += block_size;
        end += block_size;
    }

    I new_block_start = end - nEdges;
    get_minimum_reduced_costs_in_block<I, V>(begin, nEdges, costs, potentials, source, target,
                                             state, min, entering_edge);
    get_minimum_reduced_costs_in_block<I, V>(0, new_block_start, costs, potentials, source, target,
                                             state, min, entering_edge);
    if (min < 0)
    {
        start_edge = new_block_start;
        return;
    }

    begin = new_block_start;
    end = new_block_start + block_size;
    while (begin < start_edge)
    {
        get_minimum_reduced_costs_in_block<I, V>(begin, end, costs, potentials, source, target,
                                                 state, min, entering_edge);
        if (min < 0)
        {
            start_edge = end;
            return;
        }
        begin += block_size;
        end += block_size;
    }
}

template <typename I, typename V,
          typename Logger>
inline void blockSearch(          //
    const I block_size,           //
    const I nEdges,               //
    const vector<V>& costs,       //
    const vector<V>& potentials,  //
    const vector<I>& source,      //
    const vector<I>& target,      //
    const vector<State>& state,   //
    I& entering_edge,             //
    I& start_edge,                //
    Logger& logger                //
)
{
    logger.start();
    blockSearch<I, V>(block_size, nEdges, costs, potentials, source, target, state, entering_edge,
                      start_edge);
    logger.end();
    logger.increment_find_entering();
}
