#pragma once

#include "size_for_output.hpp"

#include <vector>
#include <list>
#include <map>
#include <unordered_map>

namespace utils {
/*
 * compute size for output for containers
 */

template <typename Type>
inline unsigned size_for_output(std::vector<Type> const& v) {
    return size_for_output(Type()) * v.size();
}

template <typename Type>
inline unsigned size_for_output(std::list<Type> const& v) {
    return size_for_output(Type()) * v.size();
}

template <typename Key, typename Value>
inline unsigned size_for_output(std::map<Key,Value> const& m) {
    return (size_for_output(Key()) + size_for_output(Value())) * m.size();
}

template <typename Key, typename Value>
inline unsigned size_for_output(std::unordered_map<Key,Value> const& m) {
    return (size_for_output(Key()) + size_for_output(Value())) * m.size();
}


}
