#pragma once

#include <tuple>
#include <utility>

template <typename ... Ts>
using tuple = std::tuple;

template <typename T1, typename T2>
using pair = std::pair<T1, T2>;

using std::tie;