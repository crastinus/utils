#pragma once

#include <streambuf>
#include <iosfwd>
#include <stdlib.h>
#include <string>

namespace utils {

class string_streambuf : public std::streambuf {
   public:
    string_streambuf(std::string& buf);

   private:
    int_type overflow(int_type ch);
    int sync();

    std::streamsize xsputn(char const* s, std::streamsize n);

    string_streambuf(string_streambuf const&) = delete;
    string_streambuf& operator=(string_streambuf const&) = delete;

   private:
    std::string& buffer_;
};
}

