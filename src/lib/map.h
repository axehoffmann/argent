#pragma once

#include <unordered_map>

template <typename K, typename V, int size>
using small_map = std::unordered_map<K, V>;

template <typename K, typename V>
using map = std::unordered_map<K, V>;