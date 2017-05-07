#include "extra.hpp"
#include "filesystem.hpp"
#include <utils/string/string.hpp>

namespace utils {
namespace fs {
    std::string find_in_parent_folder(std::string const& path, std::string const& filename) {
        auto join_mode = (!path.empty() && path.front() == '/' ? join_politics::LEFT_SIDE : 0);

        // extract folder
        auto fld = directory(path);
        if (fld.empty())
            return "";

        auto splitted_file_path = utils::split(fld, '/');
        splitted_file_path.emplace_back();

        do {
            splitted_file_path.back() = filename;
            auto joined_file_path     = utils::join(splitted_file_path, "/", join_mode);
            if (exists(joined_file_path))
                return joined_file_path;

            // shrink path on one node
            splitted_file_path.pop_back();
        } while (!splitted_file_path.empty());

        return "";
    }
}
}

