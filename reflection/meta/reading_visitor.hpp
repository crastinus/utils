#pragma once

namespace templater {

// current value type
enum class cv_type {
    STRING  = 1,
    INTEGER = 2,
    DOUBLE  = 3,
    BOOLEAN = 4,
    FLOAT   = 5,
    NONE    = 6,
    OBJECT  = 7, 
    ARRAY   = 8

};

struct reading_visitor {
    /*
     * Visitor for meta templater
     * TODO: Remove all strings and use string_view
     */
    reading_visitor()                       = default;
    reading_visitor(reading_visitor const&) = delete;
    virtual ~reading_visitor()              = default;

    virtual char const* read_string() noexcept        = 0;
    virtual int         read_integer() noexcept       = 0;
    virtual float       read_float() noexcept         = 0;
    virtual double      read_double() noexcept        = 0;
    virtual bool        read_boolean() noexcept       = 0;
    virtual void        null_value() noexcept         = 0;
    virtual cv_type     current_value_type() noexcept = 0;

    //prepared visitor to deserialize object and array
    //false if current object type has wrong type
    virtual bool        prepare_object() noexcept                  = 0;
    virtual void        end_object() noexcept                      = 0;
    virtual bool        extract_member() noexcept                  = 0;
    virtual char const* member_name() noexcept                     = 0;
    virtual bool extract_member_by_name(char const* name) noexcept = 0;
    virtual void        end_element() noexcept = 0;

    //prepare arrya for reading
    virtual bool prepare_array() noexcept = 0;
    virtual void end_array() noexcept     = 0;
    // false  array empty
    virtual bool     extract_element() noexcept = 0;
    virtual unsigned array_size() noexcept      = 0;
};
}
