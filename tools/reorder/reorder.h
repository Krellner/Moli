// Copyright (C) 2020 Florian Krellner
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

namespace tools {

using namespace std;

// template <typename order_iterator, typename value_iterator>
// void reorder(order_iterator order_begin, order_iterator order_end, value_iterator v) {
//     typedef typename std::iterator_traits<value_iterator>::value_type value_t;
//     typedef typename std::iterator_traits<order_iterator>::value_type index_t;
//     typedef typename std::iterator_traits<order_iterator>::difference_type diff_t;

//     diff_t remaining = order_end - 1 - order_begin;
//     for (index_t s = index_t(); remaining > 0; ++s) {
//         index_t d = order_begin[s];
//         if (d == (diff_t)-1)
//             continue;
//         --remaining;
//         value_t temp = v[s];
//         for (index_t d2; d != s; d = d2) {
//             swap(temp, v[d]);
//             swap(order_begin[d], d2 = (diff_t)-1);
//             --remaining;
//         }
//         v[s] = temp;
//     }
// }

template <typename I, typename V>
vector<V> reorder(const vector<V> &values, const vector<I> &order) {
    vector<V> new_order;
    new_order.reserve(order.size());
    for (I idx : order)
        new_order.push_back(values[idx]);
    return new_order;
}

} // namespace tools