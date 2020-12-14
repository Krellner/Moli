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

template <typename I, typename V> struct xti {
    vector<I> &last_successor,                   //
        vector<I> &number_successors,            //
        vector<I> &parent,                       //
        vector<I> &predecessor,                  //
        vector<I> &reversed_thread,              //
        vector<I> &thread,                       //
        vector<Direction> &direction_predecessor //
};