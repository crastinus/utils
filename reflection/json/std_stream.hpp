#pragma once

#include <stddef.h>
#include <assert.h>

namespace rapidjson {
template <typename std_container>
struct std_stream {
    using Ch = typename std_container::value_type;

    std_stream(std_container& container) : container_(container) {}

    Ch Peek() const {
        assert(false);
        return '\0';
    }

    Ch Take() {
        assert(false);
        return '\0';
    }

    size_t Tell() const { return 0; }

    Ch* PutBegin() {
        assert(false);
        return 0;
    }
    void Put(Ch ch) { container_.push_back(ch); }
    void        Flush() {}
    size_t      PutEnd(Ch*) {
        assert(false);
        return 0;
    }

   private:
    std_container& container_;
};
}
