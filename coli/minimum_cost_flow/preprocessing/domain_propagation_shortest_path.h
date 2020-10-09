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
#include <iostream>

namespace domain_propagation {

using namespace std;

template <typename I, typename V>
inline void domain_propagation( //
    const vector<I> &source,    //
    const vector<I> &target,    //
    vector<V> &capacity,        //
    vector<V> &supply           //
) {}

} // namespace domain_propagation