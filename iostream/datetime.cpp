#include "datetime.hpp"
//#include <date.h>

#include <time.h>
#include <iomanip>

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace utils {
namespace {
    struct tm extract_time(struct tm*(timefunction_r)(const time_t*, struct tm*)) {
        struct tm result;
        auto current = time(nullptr);
        timefunction_r(&current, &result);
        return result;
    }

    struct tm extract_local_time() {
        return extract_time(localtime_r);
    }

    struct tm extract_utc_time() {
        return extract_time(gmtime_r);
    }

    std::ostream& output_datetime(std::ostream& os, struct tm t) {
        auto fill = os.fill();
        os.fill('0');
        os << t.tm_year + 1900 << '.' << std::setw(2) << t.tm_mon + 1 << '.' << std::setw(2)
           << t.tm_mday << ' ' << std::setw(2) << t.tm_hour << ':' << std::setw(2) << t.tm_min
           << ':' << std::setw(2) << t.tm_sec;
        os.fill(fill);
        return os;
    }
}
std::ostream& datetime_local(std::ostream& os) { return output_datetime(os, extract_local_time()); }
std::ostream& datetime_utc(std::ostream& os) { return output_datetime(os, extract_utc_time()); }
}
