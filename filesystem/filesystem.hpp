#pragma once
#include <string>
#include <vector>

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
    uint64_t    file_size(std::string const& path);

    std::vector<std::string> files_in_directory(std::string const& path);

    std::string canonicalize(std::string const& path);

    std::string current_directory();

    bool move(std::string const& file_path, std::string const& new_file_path);

    // TODO: remove, remove_dir
    void remove_file(std::string const& path);

}
}
