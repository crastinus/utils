#pragma once

#include <string>

namespace utils {
namespace fs {
    // \brief
    // finding file in parent folder
    // returns path to filename or empty string if file note existed
    // \path - path to file or folder
    // \filename - filename to find
    std::string find_in_parent_folder(std::string const& path, std::string const& filename);
}
}
