#include "time.hpp"

#include <windows.h>

namespace utils {
namespace time {

static uint64_t windows_timestamp() {
    FILETIME ft;
    uint64_t tmpres = 0;
    // 100-nanosecond intervals since January 1, 1601 (UTC)
    // which means 0.1 us
    GetSystemTimeAsFileTime(&ft);
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
    // January 1st, 1970 - January 1st, 1601 UTC ~ 369 years
    // or 116444736000000000 us
    static const uint64_t deltaepoch = 116444736000000000;
    tmpres -= deltaepoch;
    return tmpres;
}

uint64_t timestamp_in_ms() {
    return windows_timestamp() / 10000;
}

} // namespace time
} // namespace utils