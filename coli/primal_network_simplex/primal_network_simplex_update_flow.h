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
inline void update_flow(const V delta,                                  //
                        const I apex,                                   //ˇ
                        const I entering_idx,                           //
                        const I entering_v,                             //
                        const I entering_w,                             //
                        const vector<I> &predecessor,                   //
                        const vector<I> &parent,                        //
                        const vector<Direction> &direction_predecessor, //
                        vector<V> &flow                                 //
) {
    // Push additional flow of delta through the cycle that is induced by the entering edge. If flow
    // on the entering edge was at its upper bound, the flow is pushed from the tail of the entering
    // edge via the apex to the head of the entering edge. If the flow on the entering edge was
    // zero, the flow is pushed in the opesite direction, i.e. from the head to the tail of the
    // entering edge.
    flow[entering_idx] += delta;
    for (I u = entering_v; u != apex; u = parent[u])
        flow[predecessor[u]] -= V(direction_predecessor[u]) * delta;
    for (I u = entering_w; u != apex; u = parent[u])
        flow[predecessor[u]] += V(direction_predecessor[u]) * delta;
}

template <typename I, typename V,
          typename Logger>
inline void update_flow(const V delta,                                  //
                        const I apex,                                   //ˇ
                        const I entering_idx,                           //
                        const I entering_v,                             //
                        const I entering_w,                             //
                        const vector<I> &predecessor,                   //
                        const vector<I> &parent,                        //
                        const vector<Direction> &direction_predecessor, //
                        vector<V> &flow,                                //
                        Logger &logger                                  //
) {
    logger.start();
    update_flow<I, V>(delta, apex, entering_idx, entering_v, entering_w, predecessor, parent,
                      direction_predecessor, flow);
    logger.end();
    logger.increment_update_flow();
}