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

template <typname LP,   //
          typename BFS, //
          typename T    //
          >
void primal_simplex_algorithm(   //
    const LP &linear_program,    //
    BFS &basic_feasible_solution //
) {
    bool found_optimum = find_entering_variable(linear_program, basic_feasible_solution);
    if (found_optimum)
        return;

    ftran(linear_program, basic_feasible_solution);

    bool is_unbounded = find_leaving_variable(linear_program, basic_feasible_solution);
    if (is_unbounded)
        return;

    update_primal_soultion(linear_program, basic_feasible_solution);

    update_basis_status(linear_program, basic_feasible_solution);
    update_basis_representation(linear_program, basic_feasible_solution);

    btran(linear_program, basic_feasible_solution);

    update_simplex_multipliers(linear_program, basic_feasible_solution);
}