#include "filesystem.hpp"
#include <fstream>

namespace utils {
namespace fs {
uint64_t file_size(std::string const& path) {
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

} // namespace fs
} // namespace utils