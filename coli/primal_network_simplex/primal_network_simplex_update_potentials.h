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

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

using namespace std;

template <typename I,
          typename V>
inline void updatePotentials(          //
    const I last_successor_in_v,       //
    const I in_v,                      //
    const I in_w,                      //
    const V oriented_costs,            //
    const I number_successors,         //
    const vector<I> &thread,           //
    const vector<I> &reversed_thread,  //
    vector<V> &potentials              //
)
{
    V reduced_costs = potentials[in_w] - potentials[in_v] - oriented_costs;

    // We update the potentials in the smaller subtree. The subtrees are induced by the entering
    // edge. We iterate simultaneously from the first and last vertex in the tree towards each
    // other and hope for multitasking in lines 1, 2, 3 and 4.
    if (2 * number_successors < potentials.size())
    {
        I u = in_v;
        I v = last_successor_in_v;

        I ctr = 0;
        while (ctr < number_successors / 2)
        {
            potentials[u] += reduced_costs;  // (1)
            potentials[v] += reduced_costs;  // (2)
            u = thread[u];
            v = reversed_thread[v];
            ctr++;
        }
        if (number_successors % 2) potentials[v] += reduced_costs;
    }
    else
    {
        I u = thread[last_successor_in_v];
        I v = reversed_thread[in_v];

        I ctr = 0;
        I nVertivces = potentials.size();
        I nCtr = (nVertivces - number_successors);
        while (ctr < nCtr / 2)
        {
            potentials[u] -= reduced_costs;  // (3)
            potentials[v] -= reduced_costs;  // (4)
            u = thread[u];
            v = reversed_thread[v];
            ctr++;
        }
        if (nCtr % 2) potentials[v] -= reduced_costs;
    }
}

template <typename I, typename V,
          typename Logger>
inline void updatePotentials(          //
    const I last_successor_in_v,       //
    const I in_v,                      //
    const I in_w,                      //
    const V oriented_costs,            //
    const I number_successors,         //
    const vector<I> &thread,           //
    const vector<I> &reversed_thread,  //
    vector<V> &potentials,             //
    Logger &logger                     //
)
{
    logger.start();
    updatePotentials<I, V>(last_successor_in_v, in_v, in_w, oriented_costs, number_successors,
                           thread, reversed_thread, potentials);
    logger.end();
    logger.increment_update_potentials();
}
