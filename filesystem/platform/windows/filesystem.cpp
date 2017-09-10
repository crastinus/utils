
#include <utils/filesystem/filesystem.hpp>
#include <utils/string/string.hpp>
//#include <unistd.h>
//#include <sys/stat.h>
#include <assert.h>
#include <windows.h>

#include <utils/string/narrow.hpp>

namespace utils {
namespace fs {

    enum class node_mode { NONE_EXISTED = 1, FILE_NODE = 2, DIRECTORY_NODE = 3 };

    node_mode get_mode(std::string const& path) {
        //struct stat info;
        //if (stat(path.data(), &info))
        //    return node_mode::NONE_EXISTED;
        //if (S_ISDIR(info.st_mode))
        //    return node_mode::DIRECTORY_NODE;
        //if (S_ISREG(info.st_mode))
        //    return node_mode::FILE_NODE;
		assert(false);
        return node_mode::NONE_EXISTED;
    }

    bool exists (std::string const& path) {

        auto file = utils::native(path);

        WIN32_FIND_DATA FindFileData;
        HANDLE handle = FindFirstFile(file.c_str(), &FindFileData);
        bool found    = (handle != INVALID_HANDLE_VALUE);
        if (found) {
            FindClose(handle);
        }
        return found;
    }
    bool is_file(std::string const& path) { return get_mode(path) == node_mode::FILE_NODE; }
    bool is_dir(std::string const& path) { return get_mode(path) == node_mode::DIRECTORY_NODE; }

    std::string filename(std::string const& path) {
        if (!is_file(path))
            return "";
        auto splited = utils::split(path, '/');
        return splited.back();
    }

    std::string extension(std::string const& path) {
        if (!is_file(path))
            return "";
        auto splited = utils::split(path, '.');
        return splited.back();
    }

    std::string directory(std::string const& path) {
        bool global_path = (!path.empty() && path.front() == '/');
        auto node = get_mode(path);

        // check obivious variants
        if (node == node_mode::NONE_EXISTED)
            return "";
        if (node == node_mode::DIRECTORY_NODE)
            return path;
        assert(node == node_mode::FILE_NODE);

        auto splitted = utils::split(path, '/');
        splitted.pop_back();

        return utils::join(splitted, "/", (global_path ? join_politics::LEFT_SIDE : 0));
    }

    std::string canonicalize(std::string const& path) {
        bool global_path = (!path.empty() && path.front() == '/');
        auto splitted = utils::split(path, '/');
        decltype(splitted) result;
        result.reserve(splitted.size());

        auto it  = splitted.begin();
        auto end = splitted.end();
        // first part of path ../../../tail/of/the/path
        while (it != end && it->compare("..") == 0) {
            result.push_back("..");
            ++it;
        }

        // path with
        while (it != end) {
            if (it->compare("..") == 0)
                result.pop_back();
            else if (it->compare(".") != 0)
                result.push_back(*it);
            ++it;
        }

        return utils::join(result, "/", (global_path ? join_politics::LEFT_SIDE : 0));
    }


    std::string current_directory() {
        
		static constexpr DWORD size = 1024;

		TCHAR buffer[size] = {};
		DWORD result_size = ::GetCurrentDirectory(size, buffer);
		if (result_size == size || result_size == 0)
			return "";

        return utils::narrow(buffer);
    }

    void remove_file(std::string const& path) {
        DeleteFile(utils::native(path).c_str());
    }
}
}
