#pragma once

#include <string>
#include <vector>

namespace utils {

enum split_politics { SKIP_EMPTY = 1 };

enum join_politics { RIGHT_SIDE = 1, LEFT_SIDE = 2 };

std::vector<std::string> split(std::string const& src, char separator,
                               int politics = SKIP_EMPTY);
std::string join(std::vector<std::string> const& str, std::string const& separator,
                 int politics = 0);
}
