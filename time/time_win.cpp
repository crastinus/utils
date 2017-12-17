#include "time.hpp"

#include <windows.h>
#include <time.h>

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

uint64_t timestamp_in_ms_for_date(int year, int month, int day, int hour, int minute, int second, int ms) {
    struct tm t;
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;
    
    uint64_t result_time = static_cast<uint64_t>(mktime(&t));
    return (result_time * 1000) + ms;
}

} // namespace time
} // namespace utils