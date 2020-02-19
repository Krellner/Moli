#pragma once

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"
#include "primal_network_simplex_update_flow.h"
#include "primal_network_simplex_update_state.h"
#include "primal_network_simplex_update_tree.h"
#include "primal_network_simplex_update_potentials.h"

#include <vector>

using namespace std;

//
//
//
//
template <typename I,
          typename V,
          typename Logger>
void update(
    const Change change,                      //
    const V delta,                            // change ... TODO
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
)
{
    if (delta > 0)
        updateFlow<I, V, Logger>(
            state[entering_idx] * delta, apex, entering_idx, entering_v, entering_w, predecessor,
            parent, direction_predecessor, flow, logger);

    updateState<I, V, Logger>(
        entering_idx, leaving_idx, change, flow, state, logger);

    // if change_type == 0 (i.e. false) then entering edge is leaving edge, nothing needs to be done
    if (change)
    {
        updateTree<I, Logger>(
            apex, in_v, in_w, length, entering_idx, entering_v, entering_w, out_v, last_successor, number_successors,
            parent, predecessor, reversed_thread, thread, direction_predecessor, logger);

        V oriented_costs = direction_predecessor[in_v] * costs[entering_idx];
        updatePotentials<I, V, Logger>(
            last_successor[in_v], in_v, in_w, oriented_costs, number_successors[in_v], thread, reversed_thread, potentials, logger);
    }
}