#include "string_streambuf.hpp"
#include <assert.h>
#include <iterator>

namespace utils {
string_streambuf::string_streambuf(std::string& buf) : buffer_(buf) {
    char* data = &buffer_.front();
    setp(data, data + buffer_.size() - 1);
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
