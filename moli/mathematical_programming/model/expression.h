#pragma once

#include "variable.h"

class Expression {};



constexpr Expression operator+(Expression lhs, Expression rhs) { return rhs + lhs; }

constexpr Expression operator+(Variable lhs, Variable rhs) { Expression{}; }

constexpr Expression operator+(Expression lhs, Variable rhs) { return lhs; }

constexpr Expression operator+(Variable lhs, Expression rhs) { return rhs + lhs; }

constexpr Expression operator+(Variable lhs, Expression rhs) { return rhs + lhs; }

template <typename T> constexpr Expression operator+(Variable lhs, T rhs) { return Expression{}; }

template <typename T> constexpr Expression operator+(T lhs, Variable rhs) { return rhs + lhs; }
