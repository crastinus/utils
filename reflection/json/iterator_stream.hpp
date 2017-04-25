#pragma once
#include <assert.h>
#include <cstddef>

namespace rapidjson {

template <typename Iterator>
struct valtype {
    using type = typename Iterator::value_type;
};

template <>
struct valtype<char const*> {
    using type = char;
};

template <>
struct valtype<char*> {
    using type = char;
};

template <typename It>
using valtype_t = typename valtype<It>::type;

template <typename iterator>
struct iterator_stream {
    using Ch = valtype_t<iterator>;

    iterator_stream(iterator begin, iterator end)
        : begin_(begin), current_(begin), end_(end) {}

    Ch Peek() const {
        if (current_ == end_)
            return '\0';
        return *current_;
    }

    Ch Take() {
        if (current_ == end_)
            return '\0';
        return *(current_++);
    }

    size_t Tell() const { return current_ - begin_; }

    Ch* PutBegin() {
        assert(false);
        return 0;
    }
    void   Put(Ch) { assert(false); }
    void   Flush() { assert(false); }
    size_t PutEnd(Ch*) {
        assert(false);
        return 0;
    }

   private:
    iterator begin_, current_, end_;
};
}
