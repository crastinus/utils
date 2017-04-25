#pragma once

#include <Meta.h>
#include "writing_visitor.hpp"
#include <vector>
#include <unordered_map>
#include "templater.hpp"

namespace templater {

template <typename Class, typename = void, typename = void>
inline void serialize(writing_visitor* visitor, Class const& obj) noexcept {
    serialize_impl(visitor, obj);
}

template <typename Class,
          typename = std::enable_if_t<meta::isRegistered<Class>()>>
inline void serialize_impl(writing_visitor* visitor,
                           Class const&     obj) noexcept {
    serialize_object(visitor, obj);
}

template <typename Class,
          typename = std::enable_if_t<!meta::isRegistered<Class>()>,
          typename = void>
inline void serialize_impl(writing_visitor* inst, Class const& obj) noexcept {
    return serialize_basic(inst, obj);
}
}
