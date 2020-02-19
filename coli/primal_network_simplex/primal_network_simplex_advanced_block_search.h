#pragma once

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

#include <vector>
#include <limits>

using namespace std;

//
//
//
//
template <typename I,
          typename V>
inline void get_minimum_advanced_reduced_costs_in_block(
    const I begin,                                  //
    const I end,                                    //
    const vector<V> &costs,                         //
    const vector<V> &potentials,                    //
    const vector<V> &flow,                          //
    const vector<V> &capacity,                      //
    const vector<I> &predecessor,                   //
    const vector<I> &source,                        //
    const vector<I> &target,                        //
    const vector<State> &state,                     //
    const vector<Direction> &direction_predecessor, //
    V &min_1,                                       //
    I &arg_min_1,                                   //
    V &min_2,                                       //
    I &arg_min_2                                    //
)
{
    for (I eIdx = begin; eIdx < end; ++eIdx)
    {
        I in_v = source[eIdx];
        I in_w = target[eIdx];
        State state_edge = state[eIdx];
        V reduced_costs_1 = V(state[eIdx]) * (costs[eIdx] + potentials[in_v] - potentials[in_w]);

        if (reduced_costs_1 < min_1)
        {
            min_1 = reduced_costs_1;
            arg_min_1 = eIdx;
        }

        if (state[eIdx] == UPPER)
            swap(in_v, in_w);

        I pred_v = predecessor[in_v];
        I pred_w = predecessor[in_w];

        I residual_capacity_pred_v = (direction_predecessor[in_v] == DOWN) ? capacity[pred_v] - flow[pred_v] : flow[pred_v];
        I residual_capacity_pred_w = (direction_predecessor[in_w] == UP) ? capacity[pred_w] - flow[pred_w] : flow[pred_w];

        V reduced_costs_2 = reduced_costs_1 * min(residual_capacity_pred_v, residual_capacity_pred_w);

        if (reduced_costs_2 < min_2)
        {
            min_2 = reduced_costs_2;
            arg_min_2 = eIdx;
        }
    }
}

//
//
//
//
template <typename I,
          typename V>
inline void advancedBlockSearch(
    const I block_size,                             // size of each block
    const I nEdges,                                 // number of edges in the original graph
    const vector<V> &costs,                         //
    const vector<V> &potentials,                    //
    const vector<V> &flow,                          //
    const vector<V> &capacity,                      //
    const vector<I> &predecessor,                   //
    const vector<I> &source,                        //
    const vector<I> &target,                        //
    const vector<State> &state,                     //
    const vector<Direction> &direction_predecessor, //
    I &entering_edge,                               //
    I &start_edge                                   // index of first edge for searching
)
{
    V min_1 = 0;
    V min_2 = 0;

    I entering_edge_1 = entering_edge;
    I entering_edge_2 = entering_edge;

    //
    //
    I begin = start_edge;
    I end = start_edge + block_size;
    while (end <= nEdges)
    {
        get_minimum_advanced_reduced_costs_in_block<I, V>(
            begin, end, costs, potentials, flow, capacity, predecessor, source, target, state, direction_predecessor,
            min_1, entering_edge_1, min_2, entering_edge_2);
        if (min_2 < 0)
        {
            entering_edge = entering_edge_2;
            start_edge = end;
            return;
        }
        if (min_1 < 0)
        {
            entering_edge = entering_edge_1;
            start_edge = end;
            return;
        }
        begin += block_size;
        end += block_size;
    }

    //
    //
    I new_block_start = end - nEdges;
    get_minimum_advanced_reduced_costs_in_block<I, V>(
        begin, nEdges, costs, potentials, flow, capacity, predecessor, source, target, state, direction_predecessor,
        min_1, entering_edge_1, min_2, entering_edge_2);
    get_minimum_advanced_reduced_costs_in_block<I, V>(
        0, new_block_start, costs, potentials, flow, capacity, predecessor, source, target, state, direction_predecessor,
        min_1, entering_edge_1, min_2, entering_edge_2);
    if (min_2 < 0)
    {
        entering_edge = entering_edge_2;
        start_edge = new_block_start;
        return;
    }
    if (min_1 < 0)
    {
        entering_edge = entering_edge_1;
        start_edge = end;
        return;
    }

    //
    //
    begin = new_block_start;
    end = new_block_start + block_size;
    while (begin < start_edge)
    {
        get_minimum_advanced_reduced_costs_in_block<I, V>(
            begin, end, costs, potentials, flow, capacity, predecessor, source, target, state, direction_predecessor,
            min_1, entering_edge_1, min_2, entering_edge_2);
        if (min_2 < 0)
        {
            entering_edge = entering_edge_2;
            start_edge = end;
            return;
        }
        if (min_1 < 0)
        {
            entering_edge = entering_edge_1;
            start_edge = end;
            return;
        }
        begin += block_size;
        end += block_size;
    }
}

template <typename I,
          typename V,
          typename Logger>
inline void avancedBlockSearch(
    const I block_size,                             // size of the block that gets searched
    const I nEdges,                                 // number of edges in the original graph
    const vector<V> &costs,                         //
    const vector<V> &potentials,                    //
    const vector<V> &flow,                          //
    const vector<V> &capacity,                      //
    const vector<I> &predecessor,                   //
    const vector<I> &source,                        //
    const vector<I> &target,                        //
    const vector<State> &state,                     //
    const vector<Direction> &direction_predecessor, //
    I &entering_edge,                               //
    I &start_edge,                                  // index of first edge for searching
    Logger &logger                                  //
)
{
    logger.start();
    advancedBlockSearch<I, V>(
        block_size, nEdges, costs, potentials, flow, capacity, predecessor, source, target, state, direction_predecessor, entering_edge, start_edge);
    logger.end();
    logger.increment_find_entering();
}
