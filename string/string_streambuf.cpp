#include "string_streambuf.hpp"
#include <assert.h>
#include <iterator>

namespace utils {
string_streambuf::string_streambuf(std::string& buf) : buffer_(buf) {
    //buffer_.resize(0);
    //setp(nullptr, nullptr);
}

std::streamsize string_streambuf::xsputn(char const* s, std::streamsize n) {
    std::copy(s, s + n, std::back_inserter(buffer_));
    return n;
}

string_streambuf::int_type string_streambuf::overflow(int_type ch) {
    buffer_.push_back(ch);
    return ch;
}

int string_streambuf::sync() {
    return -1;
}
}
