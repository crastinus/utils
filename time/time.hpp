#pragma once

#include <stdint.h>

namespace utils {
namespace time {
uint64_t timestamp_in_ms();
uint64_t timestamp_in_ms_for_date(int year, int month, int day, int hour = 0, int minute = 0,
                                  int second = 0, int ms = 0);
}
} // namespace utils