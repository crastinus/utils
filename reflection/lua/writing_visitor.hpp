#pragma once

#include "../meta/writing_visitor.hpp"
#include <vector>

extern "C"{
struct lua_State;
}

namespace lua {

struct writing_visitor : public templater::writing_visitor {

    writing_visitor(lua_State* state);
    virtual ~writing_visitor();

    /*
     * Function serialized the result of the operation
     * Exists definitions for  std::string and std::vector<char>
     */
    //template <typename BufferType>
    //BufferType serialize() const;
    //template <typename BufferType>
    //BufferType pretty_serialize() const;

  private:
    virtual void write_name(char const* name, size_t len) noexcept override;

    // TODO: Use count of elements
    virtual void start_array() noexcept override;
    virtual void end_array() noexcept override;
    virtual void prepare_element() noexcept override;

    virtual void write_string(char const* value, size_t len) noexcept override;
    virtual void write_integer(int value) noexcept override;
    virtual void write_float(float value) noexcept override;
    virtual void write_double(double value) noexcept override;
    virtual void write_boolean(bool value) noexcept override;
    virtual void write_none() noexcept override;

    // TODO: use count of elements
    //       start_object(unsigned objects_count)
    virtual void start_object() noexcept override;
    virtual void end_object() noexcept override;

    lua_State* inst_;
    //stack of array indexes
    std::vector<int> array_stack_;
    std::vector<int> object_stack_;
};
}
