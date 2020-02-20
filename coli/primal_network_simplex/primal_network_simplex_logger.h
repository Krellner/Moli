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

#include <chrono> 

struct NullLogger
{
    inline constexpr void start(){};
    inline constexpr void end(){};
    inline constexpr void clear(){};
    inline constexpr void increment_initialize(){};
    inline constexpr void increment_reorder(){};
    inline constexpr void increment_find_entering(){};
    inline constexpr void increment_find_leaving(){};
    inline constexpr void increment_update_flow(){};
    inline constexpr void increment_update_state(){};
    inline constexpr void increment_update_tree(){};
    inline constexpr void increment_update_potentials(){};
    void print(){};
};

struct PerformanceLogger
{
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::chrono::time_point<std::chrono::high_resolution_clock> _end;

    std::chrono::duration<double> time_initialize_1 = std::chrono::duration<double>(0);
    size_t ctr_find_entering = 0;
    std::chrono::duration<double> time_find_entering = std::chrono::duration<double>(0);
    size_t ctr_find_leaving = 0;
    std::chrono::duration<double> time_find_leaving = std::chrono::duration<double>(0);
    size_t ctr_update_flow = 0;
    std::chrono::duration<double> time_update_flow = std::chrono::duration<double>(0);
    size_t ctr_update_state = 0;
    std::chrono::duration<double> time_update_state = std::chrono::duration<double>(0);
    size_t ctr_update_tree = 0;
    std::chrono::duration<double> time_update_tree = std::chrono::duration<double>(0);
    size_t ctr_update_potentials = 0;
    std::chrono::duration<double> time_update_potentials = std::chrono::duration<double>(0);

    inline void start() { _start = std::chrono::high_resolution_clock::now(); };
    inline void end() { _end = std::chrono::high_resolution_clock::now(); };

    inline void increment_initialize() { time_initialize_1 += _end - _start; };

    inline constexpr void clear()
    {
        ctr_find_entering = 0;
        ctr_find_leaving = 0;
        ctr_update_flow = 0;
        ctr_update_state = 0;
        ctr_update_tree = 0;
        ctr_update_potentials = 0;
        time_initialize_1 = std::chrono::duration<double>(0);
        time_find_entering = std::chrono::duration<double>(0);
        time_find_leaving = std::chrono::duration<double>(0);
        time_update_flow = std::chrono::duration<double>(0);
        time_update_state = std::chrono::duration<double>(0);
        time_update_tree = std::chrono::duration<double>(0);
        time_update_potentials = std::chrono::duration<double>(0);
    };
    inline void increment_find_entering()
    {
        time_find_entering += _end - _start;
        ctr_find_entering++;
    };
    inline void increment_find_leaving()
    {
        time_find_leaving += _end - _start;
        ctr_find_leaving++;
    };
    inline void increment_update_flow()
    {
        time_update_flow += _end - _start;
        ctr_update_flow++;
    };
    inline void increment_update_state()
    {
        time_update_state += _end - _start;
        ctr_update_state++;
    };
    inline void increment_update_tree()
    {
        time_update_tree += _end - _start;
        ctr_update_tree++;
    };
    inline void increment_update_potentials()
    {
        time_update_potentials += _end - _start;
        ctr_update_potentials++;
    };

    void print()
    {
        cout << endl;
        auto total_time = time_initialize_1.count() + time_find_entering.count() + time_find_leaving.count() +
                          time_update_flow.count() + time_update_state.count() + time_update_tree.count() +
                          time_update_potentials.count();

        cout << "timing init 1:    " << fixed << time_initialize_1.count() << " (" << time_initialize_1.count() / total_time << ")" << endl;
        cout << "timing entering:  " << fixed << time_find_entering.count()
             << " (" << time_find_entering.count() / total_time << ", " << ctr_find_entering << ")" << endl;
        cout << "timing leaving:   " << fixed << time_find_leaving.count()
             << " (" << time_find_leaving.count() / total_time << ", " << ctr_find_leaving << ")" << endl;
        cout << "timing flow:      " << fixed << time_update_flow.count()
             << " (" << time_update_flow.count() / total_time << ", " << ctr_update_flow << ")" << endl;
        cout << "timing state:     " << fixed << time_update_state.count()
             << " (" << time_update_state.count() / total_time << ", " << ctr_update_state << ")" << endl;
        cout << "timing tree:      " << fixed << time_update_tree.count()
             << " (" << time_update_tree.count() / total_time << ", " << ctr_update_tree << ")" << endl;
        cout << "timing potential: " << fixed << time_update_potentials.count()
             << " (" << time_update_potentials.count() / total_time << ", " << ctr_update_potentials << ")" << endl;
        cout << "total time: " << total_time << endl;
        cout << endl;
    }
};