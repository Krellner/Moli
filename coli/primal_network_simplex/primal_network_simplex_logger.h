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
};