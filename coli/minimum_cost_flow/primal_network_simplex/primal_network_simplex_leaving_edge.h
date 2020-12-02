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
#include <functional>

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

namespace primal_network_simplex {

using namespace std;

template <typename I, typename V, typename Compare, Change CHG, Direction DIR>
inline void update_leaving_edge( //
    const I u,                   //
    const I eIdx,                // predecessor of u
    const V capacity,            // of the edge eIdx
    const V flow,                // of the edge eIdx
    const Direction direction,   // of the edge eIdx
    Change &change_type,         //
    I &leaving_edge,             //
    I &length,                   //
    I &out_v,                    //
    I &ctr,                      //
    V &delta                     //
) {
    V residual_capacity = (direction == DIR) ? capacity - flow : flow;
    ctr++;
    if (Compare{}(residual_capacity, delta)) {
        delta = residual_capacity;
        leaving_edge = eIdx;
        out_v = u;
        change_type = CHG;
        length = ctr;
    }
}

template <typename I,
          typename V>
inline void find_leaving_edge(                      //
    I v,                                            //
    I w,                                            //
    const vector<V> &capacity,                      //
    const vector<V> &flow,                          //
    const vector<I> &predecessor,                   //
    const vector<I> &parent,                        //
    const vector<I> &number_successors,             //
    const vector<Direction> &direction_predecessor, //
    Change &change_type,                            //
    V &delta,                                       //
    I &apex,                                        //
    I &leaving_edge,                                //
    I &length,                                      //
    I &in_v,                                        //
    I &in_w,                                        //
    I &out_v                                        //
) {
    // (v,w) is the entering edge

    in_v = v;
    in_w = w;

    I num_succ_v = number_successors[v];
    I num_succ_w = number_successors[w];

    I ctr1 = 0;
    I ctr2 = 0;

    change_type = NO;

    while (v != w) {
        if (num_succ_v < num_succ_w) {
            I eIdx = predecessor[v];
            update_leaving_edge<I, V, less<>, BEFORE, DOWN>( //
                v, eIdx, capacity[eIdx], flow[eIdx], direction_predecessor[v], change_type,
                leaving_edge, length, out_v, ctr1, delta);

            v = parent[v];
            num_succ_v = number_successors[v];
        } else {
            I eIdx = predecessor[w];
            update_leaving_edge<I, V, less_equal<>, AFTER, UP>( //
                w, eIdx, capacity[eIdx], flow[eIdx], direction_predecessor[w], change_type,
                leaving_edge, length, out_v, ctr2, delta);

            w = parent[w];
            num_succ_w = number_successors[w];
        }
    }

    apex = v;

    if (change_type == AFTER)
        swap(in_v, in_w);
}

template <typename I, typename V,
          typename Logger>
inline void find_leaving_edge(                      //
    I v,                                            //
    I w,                                            //
    const vector<V> &capacity,                      //
    const vector<V> &flow,                          //
    const vector<I> &predecessor,                   //
    const vector<I> &parent,                        //
    const vector<I> &number_successors,             //
    const vector<Direction> &direction_predecessor, //
    Change &change_type,                            //
    V &delta,                                       //
    I &apex,                                        //
    I &leaving_edge,                                //
    I &length,                                      //
    I &in_v,                                        //
    I &in_w,                                        //
    I &out_v,                                       //
    Logger &logger                                  //
) {
    logger.start();
    find_leaving_edge<I, V>(v, w, capacity, flow, predecessor, parent, number_successors,
                            direction_predecessor, change_type, delta, apex, leaving_edge, length,
                            in_v, in_w, out_v);
    logger.end();
    logger.increment_find_leaving();
}

} // namespace primal_network_simplex

