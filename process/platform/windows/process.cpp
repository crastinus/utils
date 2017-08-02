#include "process.hpp"
#include <assert.h>


namespace utils {
process_platform_impl::process_platform_impl (cli_arguments const& args, std::string const& folder) {
    (void)args;
    (void)folder;
}

process_platform_impl::~process_platform_impl () {
}

void process_platform_impl::launch () {

    assert (false);
}
void process_platform_impl::terminate () {
    assert (false);
}

int64_t process_platform_impl::wait () {
    assert (false);
    return 0;
}

int64_t process_platform_impl::pid () {
    assert (false);
    return 0;
}

} // namespace utils
