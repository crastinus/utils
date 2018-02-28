#pragma once

namespace utils {
namespace threading {

// \brief working/cancelation token
struct working_token {

    working_token(bool& b)
        : flag_(b) {
        flag_ = true;
    }

    ~working_token() { flag_ = false; }

    // allways check current flag
    explicit operator bool() const {
        return flag_;
    }

 private:
    bool& flag_;
};

} // namespace threading
} // namespace utils