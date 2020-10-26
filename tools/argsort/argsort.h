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

template <typename I, typename V> vector<I> argsort(vector<V> value) {
    vector<I> argsort(value.size());
    iota(argsort.begin(), argsort.end(), I(0));
    sort(argsort.begin(), argsort.end(), [&](I i, I j) -> bool { return value[i] < value[j]; });
    return argsort;
}

} // namespace tools