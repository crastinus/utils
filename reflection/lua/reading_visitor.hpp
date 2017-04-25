#pragma once

#include "../meta/reading_visitor.hpp"
#include <vector>

extern "C" {
struct lua_State;
}

namespace lua {

struct array_info {
    int array_index_;
    int array_size_;
};

struct reading_visitor : public templater::reading_visitor {
    reading_visitor(lua_State* lua, int stack_pos = -1);
    virtual ~reading_visitor();

    virtual char const* read_string() noexcept override;
    virtual int         read_integer() noexcept override;
    virtual float       read_float() noexcept override;
    virtual double      read_double() noexcept override;
    virtual bool        read_boolean() noexcept override;
    virtual void        null_value() noexcept override;

    virtual templater::cv_type current_value_type() noexcept override;

    // prepared visitor to deserialize object and array
    // false if current object type has wrong type
    virtual bool        prepare_object() noexcept override;
    virtual void        end_object() noexcept override;
    virtual bool        extract_member() noexcept override;
    virtual char const* member_name() noexcept override;
    virtual bool extract_member_by_name(char const* name) noexcept override;

    virtual void        end_element() noexcept override;

    // prepare arrya for reading
    virtual bool prepare_array() noexcept override;
    virtual void end_array() noexcept override;

    virtual bool extract_element() noexcept override;
    virtual unsigned array_size() noexcept override;

   private:
     void push_empty();

     lua_State* inst_;
     int lua_stack_pos_;

     // stack for controling of walking
     std::vector<array_info> array_stack_;

};
}

