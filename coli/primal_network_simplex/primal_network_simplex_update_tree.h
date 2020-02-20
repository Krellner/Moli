// Copyright (C) 2003-2013 
// Egervary Jeno Kombinatorikus Optimalizalasi Kutatocsoport
// (Egervary Research Group on Combinatorial Optimization, EGRES).
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

#include "primal_network_simplex_lib.h"
#include "primal_network_simplex_logger.h"

#include <vector>
#include <iostream>

using namespace std;

//
//
//
//
template <typename I>
inline void updateTree(
    const I apex,                            //
    const I in_v,                            //
    const I in_w,                            //
    const I length,                          //
    const I entering_idx,                    //
    const I entering_v,                      //
    const I entering_w,                      //
    const I out_v,                           //
    vector<I> &last_successor,               //
    vector<I> &number_successors,            //
    vector<I> &parent,                       //
    vector<I> &predecessor,                  //
    vector<I> &reversed_thread,              //
    vector<I> &thread,                       //
    vector<Direction> &direction_predecessor //
)
{
    const I nVertices = predecessor.size();

    const I old_reversed_thread = reversed_thread[out_v];
    const I old_number_successors = number_successors[out_v];
    const I old_last_successor = last_successor[out_v];
    const I out_w = parent[out_v];

    // check if in_v and out_v coincide
    if (in_v == out_v)
    {
        // update parents, predecessors, direction_predecessor
        parent[in_v] = in_w;
        predecessor[in_v] = entering_idx;
        direction_predecessor[in_v] = (in_v == entering_v) ? UP : DOWN;

        // update thread and reversed_thread
        if (thread[in_w] != out_v)
        {
            I after = thread[old_last_successor];
            thread[old_reversed_thread] = after;
            reversed_thread[after] = old_reversed_thread;
            after = thread[in_w];
            thread[in_w] = out_v;
            reversed_thread[out_v] = in_w;
            thread[old_last_successor] = after;
            reversed_thread[after] = old_last_successor;
        }
    }
    else
    {
        // ----------------------------------------------------------------------------------------------
        // update thread, reversed_thread and parents
        // ----------------------------------------------------------------------------------------------

        // handle the case old_reversed_thread == in_w, (i.e. apex == out_w)
        I thread_continue = (old_reversed_thread == in_w) ? thread[old_last_successor] : thread[in_w];

        // remember vertices to update missing reversed_thread links between unchanged tree parts
        // between in_v and out_v (link between missing_links[2*i] and missing_links[2*i+1])
        vector<I> missing_link = {in_v, in_w};
        missing_link.reserve(2 * length);

        I v = in_v, new_parent_v = in_w;
        I last = last_successor[v];
        I before, after = thread[last];
        I last_successor_v = last_successor[v], last_successor_parent_v;

        // update parents, thread and reversed_thread (where possible) by reversing the parent-relation 
        // of the vertices from in_v to out_v in the current basis-tree (the one that is modified) and
        // modifing thread and reversed_thread accordingly
        thread[in_w] = in_v;
        while (v != out_v)
        {
            I next_v = parent[v];
            parent[v] = new_parent_v;

            // connect the subtree of v to the subtree of parent[v], corresponding reversed thread
            // reversed thread cannot be updated yet
            thread[last] = next_v;
            missing_link.push_back(next_v);
            missing_link.push_back(last);    
            
            // reconnect the subtree (the subtree after and before the subtree of v)
            before = reversed_thread[v];
            thread[before] = after; // thread[last]
            reversed_thread[after] = before;

            last_successor_parent_v = last_successor_v;
            last_successor_v = last_successor[next_v];

            last = (last_successor_v == last_successor_parent_v)
                       ? before // = reversed_thread[v]
                       : last_successor_v;

            after = thread[last];

            new_parent_v = v;
            v = next_v;
        }
        parent[out_v] = new_parent_v;
        thread[last] = thread_continue;
        reversed_thread[thread_continue] = last;
        last_successor[out_v] = last;

        // remove the subtree of out_v from the thread list
        // exepct if old_reversed_thread == in_w
        if (old_reversed_thread != in_w)
        {
            thread[old_reversed_thread] = after;
            reversed_thread[after] = old_reversed_thread;
        }

        // update remaining reversed thread using missing_link
        // reversed thread is updated between unchanged
        for (I idx = 0; idx < missing_link.size(); idx += 2)
            reversed_thread[missing_link[idx]] = missing_link[idx + 1];

        // ----------------------------------------------------------------------------------------------
        // update predecessor, direction_predecessor, last_successors, number_successor
        // ----------------------------------------------------------------------------------------------

        I tmp_sc = 0, tmp_ls = last;
        for (I u = out_v, p = parent[u]; u != in_v; u = p, p = parent[u])
        {
            predecessor[u] = predecessor[p];
            direction_predecessor[u] = Direction(-direction_predecessor[p]);
            tmp_sc += number_successors[u] - number_successors[p];
            number_successors[u] = tmp_sc;
            last_successor[p] = tmp_ls;
        }
        predecessor[in_v] = entering_idx;
        direction_predecessor[in_v] = (in_v == entering_v) ? UP : DOWN;
        number_successors[in_v] = old_number_successors;
    }

    // ----------------------------------------------------------------------------------------------
    // update last successor
    // ----------------------------------------------------------------------------------------------

    I up_limit_out = (last_successor[apex] == in_w) ? apex : nVertices;
    I last_successor_out = last_successor[out_v];

    // update last successor for all vertices that have in_w as their last successor
    // (i.e. update last successor from in_w to the root if necessary)
    for (I u = in_w; last_successor[u] == in_w && u != nVertices; u = parent[u])
        last_successor[u] = last_successor_out;

    // update last_successors from out_w to the root
    if (apex != old_reversed_thread && in_w != old_reversed_thread)
    {
        for (I u = out_w;
             u != up_limit_out && last_successor[u] == old_last_successor;
             u = parent[u])
            last_successor[u] = old_reversed_thread;
    }
    else if (last_successor_out != old_last_successor)
    {
        for (I u = out_w;
             u != up_limit_out && last_successor[u] == old_last_successor;
             u = parent[u])
            last_successor[u] = last_successor_out;
    }

    // ----------------------------------------------------------------------------------------------
    // update number of successors
    // ----------------------------------------------------------------------------------------------

    // update number_successor form in_w to apex
    for (I u = in_w; u != apex; u = parent[u])
        number_successors[u] += old_number_successors;

    // update number_successor form out_w to apex
    for (I u = out_w; u != apex; u = parent[u])
        number_successors[u] -= old_number_successors;
}

//
//
//
//
template <typename I, typename Logger>
inline void updateTree(
    const I apex,                             //
    const I in_v,                             //
    const I in_w,                             //
    const I length,                           //
    const I entering_idx,                     //
    const I entering_v,                       //
    const I entering_w,                       //
    const I out_v,                            //
    vector<I> &last_successor,                //
    vector<I> &number_successors,             //
    vector<I> &parent,                        //
    vector<I> &predecessor,                   //
    vector<I> &reversed_thread,               //
    vector<I> &thread,                        //
    vector<Direction> &direction_predecessor, //
    Logger &logger                            //
)
{
    logger.start();
    updateTree<I>(
        apex, in_v, in_w, length, entering_idx, entering_v, entering_w, out_v, last_successor,
        number_successors, parent, predecessor, reversed_thread, thread, direction_predecessor);
    logger.end();
    logger.increment_update_tree();
}