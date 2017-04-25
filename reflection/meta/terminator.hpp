#pragma once

namespace templater {
template<typename T>
struct static_assert_workaround_wrapper {
    enum {
        value = false
    };
};

template <typename T>
inline void terminate_compilation() {
    static_assert(static_assert_workaround_wrapper<T>::value,
                  "Function for a template type doesn't defined.");
}

}
