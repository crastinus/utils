#pragma once
#include <string>

namespace utils {
namespace fs {

    bool exists(std::string const& path);
    bool is_file(std::string const& path);
    bool is_dir(std::string const& path);

    void mkdir(std::string const& path);
    void mkdir_for_file(std::string const path);

    std::string filename(std::string const& path);
    std::string extension(std::string const& path);
    std::string directory(std::string const& path);

    std::string canonicalize(std::string const& path);

    std::string current_directory();

    // TODO: remove, remove_dir
    void remove_file(std::string const& path);

}
}
