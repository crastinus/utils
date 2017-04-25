#pragma once

#include "size_for_output.hpp"
#include "string_streambuf.hpp"
#include <ostream>

namespace utils {

template <typename... Args>
std::string build_string(Args&&... args) {
    std::string result;
    unsigned    result_size = 0;

    // compute output string size
    using swallow = int[];
    (void)swallow{(result_size += size_for_output(args), 0)...};

    // allocate buffer for result
    result.reserve(result_size);

    string_streambuf sb(result);
    std::ostream     os(&sb);

    (void)swallow{(os << std::forward<Args>(args), 0)...};

    return result;
}
}
