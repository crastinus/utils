#include "datetime.hpp"
//#include <date.h>

#include <time.h>
#include <iomanip>

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace utils {
namespace {

#ifdef __linux__
	typedef struct tm*(timefunction_ft)(const time_t*, struct tm*);
#define LOCALTIME localtime_r
#define GMTIME	gmtime_r
#elif _MSC_VER
	typedef errno_t (timefunction_ft)( struct tm*, const time_t*);
#define LOCALTIME localtime_s
#define GMTIME	gmtime_s
#else
	static_assert(false, "Unsupported toolset");
#endif

    struct tm extract_time(timefunction_ft timefunction_r) {
        struct tm result;
        auto current = time(nullptr);
#ifdef __linux__
        timefunction_r(&current, &result);
#elif _MSC_VER
		timefunction_r( &result, &current);
#endif
        return result;
    }

    struct tm extract_local_time() {
        return extract_time(LOCALTIME);
    }

    struct tm extract_utc_time() {
        return extract_time(GMTIME);
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
