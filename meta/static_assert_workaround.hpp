#pragma once

// workaround for cheking templates that cannot be instantiated
template<typename T>
struct static_assert_workaround_wrapper {
    enum {
        value = false
    };
};

template <typename T>
inline void static_assert_workaround() {
    static_assert(static_assert_workaround_wrapper<T>::value,
                  "Function for a template type doesn't defined.");
}
