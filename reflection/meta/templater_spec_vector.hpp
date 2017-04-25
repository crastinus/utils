#pragma once
#include "reading_visitor.hpp"
#include "writing_visitor.hpp"
#include <vector>
#include <Meta.h>

namespace templater {
// specialization for std::vector
template <typename T>
inline void deserialize_basic(reading_visitor* visitor,
                              std::vector<T>&  obj) noexcept {
    obj.clear();
    if (!visitor->prepare_array())
        return;

    while (visitor->extract_element()) {
        T result;
        deserialize(visitor, result);
        // TODO:do something withit. Might be bad_alloc exception here
        obj.push_back(std::move(result));
    }

    visitor->end_array();
}

// specialization for std::vector
template <typename T>
inline void serialize_basic(writing_visitor*      visitor,
                            std::vector<T> const& obj) {
    visitor->start_array();
    for (auto const& v : obj) {
        /*
         * next element might be object or any other basic element.
         * Send notification to visitor use this element as array item.
         */
        visitor->prepare_element();
        serialize(visitor, v);
    }
    visitor->end_array();
}
}
