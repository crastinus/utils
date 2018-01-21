
#include <utils/filesystem/filesystem.hpp>
#include <utils/string/string.hpp>
#include <utils/string/build.hpp>
//#include <unistd.h>
//#include <sys/stat.h>
#include <assert.h>
#include <windows.h>
#include <Shlobj.h>
#include <algorithm>

#include <utils/string/narrow.hpp>

namespace utils {
namespace fs {

    enum class node_mode { NONE_EXISTED = 1, FILE_NODE = 2, DIRECTORY_NODE = 3 };

    std::string handle_slashes(std::string const& input) {
        std::string result;
        result.reserve(input.size());

        bool prev_slash = false;
        for (char const& c : input) {
            if (c == '\\' || c == '/') {
                if (!prev_slash) {
                    result.push_back('\\');
                }
                prev_slash = true;
                continue;
            }
            result.push_back(c);
            prev_slash = false;
        }
        return result;
    }

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

    namespace {

    template <typename FilterFunction>
    std::vector<std::string> elements_in_directory(std::string const& path, FilterFunction&& f) {
        std::vector<std::string> result;

        auto file = utils::native(handle_slashes(path));

        WIN32_FIND_DATA find_file_data;
        HANDLE          handle = FindFirstFile(file.c_str(), &find_file_data);

        if (handle == INVALID_HANDLE_VALUE) {
            return result;
        }

        do {

            std::string file_node_name = utils::narrow(find_file_data.cFileName);
            int         attributes     = find_file_data.dwFileAttributes;

            if (!f(file_node_name, attributes))
                continue;


            result.push_back(file_node_name);
        } while (FindNextFile(handle, &find_file_data));
        return result;
    }
    } // namespace

    std::vector<std::string> files_in_directory(std::string const& path) {
        return elements_in_directory(path, [](std::string const&, int attr) { return (attr & FILE_ATTRIBUTE_DIRECTORY) == 0; });
    }

    std::vector<std::string> subdirectories(std::string const & path) {
        return elements_in_directory(path, [](std::string const& folder_name, int attr) {
            return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0 && folder_name.compare(".") != 0 &&
                   folder_name.compare("..") != 0;
        });
    }

    bool is_file(std::string const& path) { return get_mode(path) == node_mode::FILE_NODE; }
    bool is_dir(std::string const& path) { return get_mode(path) == node_mode::DIRECTORY_NODE; }

    std::string filename(char const* raw_path) {

        //TODO: try to remove on handle slashes
        std::string path = raw_path;

        for (char& c : path) {
            if (c == '\\')
                c = '/';
        }
        
        auto splited = utils::split(path, '/');
        return splited.back();
    }

    std::string extension(std::string const& path) {
        //if (!is_file(path))
        //    return "";
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

    void mkdir(std::string const& path) {
        //CreateDirectory(utils::native(path).c_str(), NULL);
        auto native_path = utils::native(path);
        int result = SHCreateDirectoryEx(NULL, native_path.c_str(), NULL);
        if (result != ERROR_SUCCESS && result != ERROR_FILE_EXISTS && result != ERROR_ALREADY_EXISTS) {
            throw std::runtime_error(build_string("Cannot create directory ", path, ". SHCreateDirectoryEx error code: ", result));
        }
    }

    void mkdir_for_file(std::string const path) {

        std::string file_path = path;
        for (auto& c : file_path)
            if (c == '/')
                c = '\\';

        // split and remove file
        auto splitted = utils::split(file_path, '\\');
        splitted.pop_back();

        // create directory
        auto new_path = utils::join(splitted, "\\");
        
        if (exists(new_path)) {
            return;
        }

        mkdir(new_path);

        printf("Create path %s\n", new_path.c_str());

    }

    bool move(std::string const& file_path_input, std::string const& new_file_path_input) {
        auto file_path = utils::native(file_path_input);
        auto new_file_path = utils::native(new_file_path_input);

        auto result = MoveFile(file_path.c_str(), new_file_path.c_str());
        return result != FALSE;
    }


}
}
