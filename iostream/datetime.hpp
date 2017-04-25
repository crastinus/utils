#pragma once
#include <ostream>

namespace utils {
std::ostream& datetime_local(std::ostream& os);
std::ostream& datetime_utc(std::ostream& os);
}
