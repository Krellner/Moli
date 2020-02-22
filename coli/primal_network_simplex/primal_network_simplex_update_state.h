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

//
//
//
//
template <typename I,
          typename V>
inline void updateState(    //
    const I entering_idx,   //
    const I leaving_idx,    //
    const Change change,    //
    const vector<V> &flow,  //
    vector<State> &state    //
)
{
    if (change)
    {
        state[entering_idx] = TREE;
        state[leaving_idx] = (flow[leaving_idx] == 0) ? LOWER : UPPER;
    }
    else
    {
        state[entering_idx] = State(-state[entering_idx]);
    }
}

template <typename I, typename V,
          typename Logger>
inline void updateState(    //
    const I entering_idx,   //
    const V leaving_idx,    //
    const Change change,    //
    const vector<V> &flow,  //
    vector<State> &state,   //
    Logger &logger          //
)
{
    logger.start();
    updateState<I, V>(entering_idx, leaving_idx, change, flow, state);
    logger.end();
    logger.increment_update_state();
}
