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

#include <iostream>

template <typename V,                                                                //
          typename I,                                                                //
          typename Instance,                                                         //
          typename Tree,                                                             //
          typename Basis,                                                            //
          typename Parameter,                                                        //
          void (*find_entering_edge)(const Instance &, const Tree &, const Basis &), //
          void (*find_leaving_edge)(const Instance &, const Tree &, const Basis &)   //
          >
std::vector<V> primal_network_simplex( //
    Instance &instance,                //
    Tree &tree,                        //
    Basis &basis,                      //
    Parameter &parameter               //
) {

    while (true) {
        find_entering_edge(instance, tree, basis, parameter);

        find_leaving_edge(instance, tree, basis, parameter);
    }
}
