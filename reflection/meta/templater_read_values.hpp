
#include <Meta.h>
#include "reading_visitor.hpp"
#include <vector>
#include <unordered_map>
#include "templater.hpp"

namespace templater {

// use this function for deserialization operations in case with new
// serializable types or add end_element function for every deserialization
template <typename Class, typename = void, typename = void>
inline void deserialize(reading_visitor* visitor, Class& obj) noexcept {
    deserialize_impl(visitor, obj);

    // end deserialization operation for element
    visitor->end_element();
}

template <typename Class, typename = std::enable_if_t<meta::isRegistered<Class>()>>
inline void deserialize_impl(reading_visitor* visitor, Class& obj) noexcept {
    deserialize_object(visitor, obj);
}

template <typename Class,
          typename = std::enable_if_t<!meta::isRegistered<Class>()>,
          typename = void>
inline void deserialize_impl(reading_visitor* inst, Class& obj) noexcept {
    return deserialize_basic(inst, obj);
}
}
