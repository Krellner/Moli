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

namespace graph {

using namespace std;

template <typename I>
vector<I> star(             //
    const I nVertices,      //
    const vector<I> &source //
) {
    I nEdges = source.size();
    vector<I> star(nVertices + 1, nEdges);
    for (I eIdx = 0; eIdx < nEdges; ++eIdx) {
        I v = source[eIdx];
        star[v] = min(star[v], eIdx);
    }
    return star;
}

} // namespace graph