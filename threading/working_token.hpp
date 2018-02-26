#pragma once

namespace utils {
namespace threading {

struct working_token {

    working_token(bool& b)
        : flag_(b) {
        flag_ = true;
    }

    ~working_token() { flag_ = false; }

 private:
    bool& flag_;
};

} // namespace threading
} // namespace utils