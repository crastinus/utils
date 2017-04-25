#pragma once
#include <assert.h>
#include <string>

namespace templater {

struct writing_visitor {
    /*
     * Visitor for meta templater
     * TODO: Remove all strings and use string_view
     */
    writing_visitor()                       = default;
    writing_visitor(writing_visitor const&) = delete;
    virtual ~writing_visitor()              = default;

    virtual void write_name(char const* name, size_t len) noexcept = 0;

    /*
     * Functions for array
     */
    virtual void start_array() noexcept     = 0;
    virtual void end_array() noexcept       = 0;
    virtual void prepare_element() noexcept = 0;

    virtual void write_string(char const* value, size_t len) noexcept = 0;
    virtual void write_integer(int value) noexcept   = 0;
    virtual void write_float(float value) noexcept   = 0;
    virtual void write_double(double value) noexcept = 0;
    virtual void write_boolean(bool value) noexcept  = 0;
    virtual void write_none() noexcept               = 0;

    /*
     * Object functions
     */
    virtual void start_object() noexcept = 0;
    virtual void end_object() noexcept   = 0;
};

template <typename StringType>
inline void write_name(writing_visitor*, StringType const&) {
    assert(false);
}

template <>
inline void write_name<std::string>(writing_visitor*   visitor,
                                    std::string const& value) {
    visitor->write_name(value.c_str(), value.length());
}

template <>
inline void write_name<char const*>(writing_visitor*   visitor,
                                    char const* const& value) {
    visitor->write_name(value, std::char_traits<char>::length(value));
}

inline void write_name(writing_visitor*   visitor,
                       char const* const& value,
                       size_t             length) {
    visitor->write_name(value, length);
}
}
