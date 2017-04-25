#pragma once

#include "../meta/writing_visitor.hpp"

namespace rj {

struct writing_visitor_impl;
struct writing_visitor : public templater::writing_visitor {

    writing_visitor();
    virtual ~writing_visitor();

    /*
     * Function serialized the result of the operation
     * Exists definitions for  std::string and std::vector<char>
     */
    template <typename BufferType>
    BufferType serialize() const;
    template <typename BufferType>
    BufferType pretty_serialize() const;

  private:
    virtual void write_name(char const* name, size_t len) noexcept override;

    virtual void start_array() noexcept override;
    virtual void end_array() noexcept override;
    virtual void prepare_element() noexcept override;

    virtual void write_string(char const* value, size_t len) noexcept override;
    virtual void write_integer(int value) noexcept override;
    virtual void write_float(float value) noexcept override;
    virtual void write_double(double value) noexcept override;
    virtual void write_boolean(bool value) noexcept override;
    virtual void write_none() noexcept override;

    virtual void start_object() noexcept override;
    virtual void end_object() noexcept override;

    writing_visitor_impl* impl_;
};
}
