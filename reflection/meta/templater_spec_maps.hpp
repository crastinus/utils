#pragma once

#include <map>
#include <unordered_map>
#include "reading_visitor.hpp"
#include "writing_visitor.hpp"
#include <Meta.h>

namespace templater {

template <typename KVType>
inline void serialize_key_value_basic(writing_visitor* visitor,
                                      KVType const&    obj) noexcept {
    visitor->start_object();
    for (auto const& p : obj) {
        write_name(visitor, p.first);
        serialize(visitor, p.second);
    }
    visitor->end_object();
}

// specialization for std::unodered_map
template <typename K, typename V>
inline void serialize_basic(writing_visitor* visitor,
                            std::unordered_map<K, V> const& obj) noexcept {
    serialize_key_value_basic(visitor, obj);
}

template <typename K, typename V>
inline void serialize_basic(writing_visitor* visitor,
                            std::map<K, V> const& obj) noexcept {
    serialize_key_value_basic(visitor, obj);
}

template <typename KVType>
inline void deserialize_key_value_basic(reading_visitor* visitor,
                                        KVType&          obj) noexcept {
    obj.clear();
    if (!visitor->prepare_object())
        return;

    while (visitor->extract_member()) {
        //        typename KVType::value_type value;
        typename KVType::key_type   key(visitor->member_name());

        auto& value = obj[key];

        deserialize(visitor, value);
        //obj[key] = std::move(value);
    }
    visitor->end_object();
}

// specialization for std::unodered_map
template <typename K, typename V>
inline void deserialize_basic(reading_visitor* visitor,
                              std::unordered_map<K, V>& obj) noexcept {
    deserialize_key_value_basic(visitor, obj);
}

template <typename K, typename V>
inline void deserialize_basic(reading_visitor* visitor,
                              std::map<K, V>& obj) noexcept {
    deserialize_key_value_basic(visitor, obj);
}
}
