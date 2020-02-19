#pragma once

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

#include <vector>

using namespace std;

//
//
//
//
template <typename I,
          typename V>
inline void updateFlow(
    const V value,                                  // value for the augmentation
    const I apex,                                   //ˇ
    const I entering_idx,                           //
    const I entering_v,                             //
    const I entering_w,                             //
    const vector<I> &predecessor,                   //
    const vector<I> &parent,                        //
    const vector<Direction> &direction_predecessor, //
    vector<V> &flow                                 //
)
{
    flow[entering_idx] += value;
    for (I u = entering_v; u != apex; u = parent[u])
        flow[predecessor[u]] -= V(direction_predecessor[u]) * value;
    for (I u = entering_w; u != apex; u = parent[u])
        flow[predecessor[u]] += V(direction_predecessor[u]) * value;
}

template <typename I,
          typename V,
          typename Logger>
inline void updateFlow(
    const V value,                                  // value for the augmentation
    const I apex,                                   //ˇ
    const I entering_idx,                           //
    const I entering_v,                             //
    const I entering_w,                             //
    const vector<I> &predecessor,                   //
    const vector<I> &parent,                        //
    const vector<Direction> &direction_predecessor, //
    vector<V> &flow,                                //
    Logger &logger                                  //
)
{
    logger.start();
    updateFlow<I, V>(
        value, apex, entering_idx, entering_v, entering_w, predecessor, parent, direction_predecessor, flow);
    logger.end();
    logger.increment_update_flow();
}