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
#include "primal_network_simplex_update_flow.h"
#include "primal_network_simplex_update_potentials.h"
#include "primal_network_simplex_update_state.h"
#include "primal_network_simplex_update_tree.h"

namespace primal_network_simplex {

using namespace std;

template <typename I, typename V,
          typename Logger>
void update(                                  //
    const Change change,                      //
    const V delta,                            //
    const I apex,                             //
    const I entering_idx,                     //
    const I entering_v,                       //
    const I entering_w,                       //
    const I leaving_idx,                      //
    const I length,                           //
    const I in_v,                             //
    const I in_w,                             //
    const I out_v,                            //
    vector<V> &costs,                         //
    vector<V> &potentials,                    //
    vector<V> &flow,                          //
    vector<I> &last_successor,                //
    vector<I> &number_successors,             //
    vector<I> &parent,                        //
    vector<I> &predecessor,                   //
    vector<I> &reversed_thread,               //
    vector<I> &thread,                        //
    vector<Direction> &direction_predecessor, //
    vector<State> &state,                     //
    Logger &logger                            //
) {
    if (delta > 0)
        update_flow<I, V, Logger>(state[entering_idx] * delta, apex, entering_idx, entering_v,
                                  entering_w, predecessor, parent, direction_predecessor, flow,
                                  logger);

    update_state<I, V, Logger>(entering_idx, leaving_idx, change, flow, state, logger);

    // if change_type == 0 (i.e. false) then entering edge is leaving edge,
    // nothing needs to be done
    if (change) {
        update_tree<I, Logger>(apex, in_v, in_w, length, entering_idx, entering_v, entering_w,
                               out_v, last_successor, number_successors, parent, predecessor,
                               reversed_thread, thread, direction_predecessor, logger);

        V oriented_costs = direction_predecessor[in_v] * costs[entering_idx];
        update_potentials<I, V, Logger>(last_successor[in_v], in_v, in_w, oriented_costs,
                                        number_successors[in_v], thread, reversed_thread,
                                        potentials, logger);
    }
}

} // namespace primal_network_simplex