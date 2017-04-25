#pragma once
#include <utility>

namespace utils {
template <typename FunctionType, typename... Arguments>
void for_each_argument(FunctionType&& function, Arguments&&... args) {
    using swallow = int[];
    (void)swallow{(function(std::forward<Arguments>(args)), 0)...};
}
}
