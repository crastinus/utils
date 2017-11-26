#pragma once

#include <string>

/*
 * Functions compute size for output strings
 */
namespace utils {
template <typename Type>
inline size_t size_for_output(Type const&) {
    return sizeof(Type) * 2;
}

template <typename ReturnType, typename... ArgsType>
inline size_t size_for_output(ReturnType (*)(ArgsType...)) {
    return 11; //unknown size. Using average
}

template <>
inline size_t size_for_output<char>(char const&) {
    return 1;
}

template <typename Type, unsigned N>
inline size_t size_for_output(Type const (&)[N]) {
    return size_for_output(Type()) * N;
}

template<>
inline size_t size_for_output<std::string>(std::string const& s) {
    return s.size();
}

}
