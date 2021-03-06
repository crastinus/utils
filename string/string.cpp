#include "string.hpp"
//#include "string_algo.hpp"
#include "string_streambuf.hpp"
#include <ostream>
#include <algorithm> 
#include <ctype.h>

namespace utils {

std::vector<std::string> split(std::string const& src, char separator,
                               int politics) {
    std::vector<std::string> result;
    result.reserve(src.size() / 8);
    
    bool skip_empty = ((politics & SKIP_EMPTY) != 0);
    bool trip_left  = ((politics & TRIP_LEFT) != 0);

    //size_t pos = 0;
    //size_t first = 0;
    auto it = src.begin();
    auto end = src.end();

    //it = (*it == separator ? it+1 : it);

    while (it != end){

        // this one must be delete every possible empty string
        if (trip_left)
            while (it != end && ::isspace(*it)) ++it;

        auto newit = std::find(it, end, separator);
        if (skip_empty && std::distance(it,newit) == 0) {
            if (newit == end)
                break;
            else
                it = newit + 1;
            continue;
        }

        result.emplace_back(it, newit);
        it = (newit == end ? end : newit + 1);
    }

    return result;
}

std::string join(std::vector<std::string> const& arr,
                 std::string const& separator, int politics) {
    std::string result;
    // allocate buffer for result
    result.reserve(arr.size() * 16);

    string_streambuf sb(result);
    std::ostream     os(&sb);

    bool left_side  = ((politics & LEFT_SIDE) != 0);
    bool right_side = ((politics & RIGHT_SIDE) != 0);

    if (left_side)
        os << separator;

    for (auto str : arr)
        os << str << separator;

    if (!right_side && result.size() > 0)
        result.resize(result.size() - separator.size());

    return result;
}

void trim_right(std::string& str) {
    while(::isspace(str.back()))
        str.pop_back();
}

std::string trim_right_im(std::string const& str) {
    auto begin = str.begin();
    auto end = str.begin() + str.size();
    if (end == begin)
        return "";

    while (end != begin && ::isspace(*(end - 1)))
        --end;

    return std::string(begin, end);
}


}

