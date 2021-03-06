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

namespace primal_network_simplex {

// state of the edge
// UPPER = -1, edge is not in the basis and at its upper bound
// TREE = 0,   egde is part of the basis, i.e. in the spanning tree
// LOWER = 1,  edge is not in the basis and at its lower bound
enum State { UPPER = -1, TREE = 0, LOWER = 1 };

// direction of the edge in the tree
// DOWN = -1, edge is facing downwards, i.e. its facing towards a leave and away from the root
// UP = 1,    edge is facing upwards, i.e. its facing towards the root
enum Direction { DOWN = -1, UP = 1 };

// available pivot rules
enum PivotRule { BLOCK_SEARCH };

//
enum Change { BEFORE = -1, NO = 0, AFTER = 1 };

} // namespace primal_network_simplex