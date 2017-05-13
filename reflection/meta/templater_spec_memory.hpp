#pragma once

#include <memory>
#include "reading_visitor.hpp"
#include "writing_visitor.hpp"
#include <Meta.h>

namespace templater {

template <typename Class>
inline void deserialize_basic(reading_visitor*, Class&) noexcept;
template <typename Class>
inline void serialize_basic(writing_visitor*, Class const&) noexcept;

template <typename Pointee>
inline void deserialize_basic(reading_visitor* visitor,
                              std::shared_ptr<Pointee>& obj) noexcept {
    obj = std::make_shared<Pointee>();
    deserialize(visitor, *obj);
}

template <typename Pointee>
inline void deserialize_basic(reading_visitor* visitor,
                              std::unique_ptr<Pointee>& obj) noexcept {
    obj = std::make_unique<Pointee>();
    deserialize(visitor, *obj);
}

template <typename pointee>
inline void serialize_basic(writing_visitor* visitor,
                            std::shared_ptr<pointee> const& obj) noexcept {
    if (obj) {
        serialize(visitor, *obj);
    } else
        visitor->write_none();
}

template <typename pointee>
inline void serialize_basic(writing_visitor* visitor,
                            std::unique_ptr<pointee> const& obj) noexcept {
    if (obj) {
        serialize(visitor, *obj);
    } else
        visitor->write_none();
}
}
