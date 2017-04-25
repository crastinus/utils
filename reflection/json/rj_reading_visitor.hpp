#pragma once

#include "../meta/reading_visitor.hpp"

namespace rj {

struct reading_visitor_impl;
struct reading_visitor : public templater::reading_visitor {
    reading_visitor();
    virtual ~reading_visitor();

    /*
     * Parsed value into rapidjson object
     * Exists definitions for  std::string and  std::vector<char>
     */
    template <typename BufferType>
    bool parse(BufferType const& buffer);

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
//    virtual void        end_object() noexcept override;
    virtual bool        extract_member() noexcept override;
    virtual char const* member_name() noexcept override;
    virtual bool extract_member_by_name(char const* name) noexcept override;

    virtual void        end_element() noexcept override;

    // prepare arrya for reading
    virtual bool prepare_array() noexcept override;
    virtual void end_array() noexcept override;
    // false  array empty
    virtual bool     extract_element() noexcept override;
    virtual unsigned array_size() noexcept override;

   private:
    reading_visitor_impl* impl_;
};
}

