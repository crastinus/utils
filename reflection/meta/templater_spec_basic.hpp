#pragma once

#include "reading_visitor.hpp"
#include "writing_visitor.hpp"
#include "terminator.hpp"
#include <string>
#include <Meta.h>

namespace templater {

// DESERIALIZATION
template <typename Class>
inline void deserialize_basic(reading_visitor* , Class& ) noexcept {
    // if class added deginition for deserialization must be present
    terminate_compilation<Class>();
}

template <>
inline void deserialize_basic(reading_visitor* visitor, std::string& value) noexcept {
    value = visitor->read_string();
}

template <>
inline void deserialize_basic(reading_visitor* visitor, int& i) noexcept {
    i = visitor->read_integer();
}

template <>
inline void deserialize_basic(reading_visitor* visitor, unsigned& i) noexcept {
    i = visitor->read_integer();
}

template <>
inline void deserialize_basic(reading_visitor* visitor, bool& b) noexcept {
    b = visitor->read_boolean();
}

template <>
inline void deserialize_basic(reading_visitor* visitor, double& d) noexcept {

    d = visitor->read_double();
}

template <>
inline void deserialize_basic(reading_visitor* visitor, float& f) noexcept {
    f = visitor->read_float();
}


//  SERIALIZATION
template <typename Class>
inline void serialize_basic(writing_visitor*, Class const&) noexcept {
    // if class added deginition for serialization must be present
    terminate_compilation<Class>();
}

template <>
inline void serialize_basic(writing_visitor* visitor, std::string const& value)  noexcept{
    visitor->write_string(value.c_str(), value.size());
}

template <>
inline void serialize_basic(writing_visitor* visitor,char const* const& value)  noexcept{
    visitor->write_string(value, std::char_traits<char>::length(value));
}

template <>
inline void serialize_basic(writing_visitor* visitor, int const& i)  noexcept{
    visitor->write_integer(i);
}

template <>
inline void serialize_basic(writing_visitor* visitor, unsigned const& i)  noexcept{
    visitor->write_integer(i);
}

template <>
inline void serialize_basic(writing_visitor* visitor, bool const& i) noexcept {
    visitor->write_boolean(i);
}

template <>
inline void serialize_basic(writing_visitor* visitor,
                            double const& i) noexcept {
    visitor->write_double(i);
}

template <>
inline void serialize_basic(writing_visitor* visitor, float const& i) noexcept {
    visitor->write_float(i);
}



}
