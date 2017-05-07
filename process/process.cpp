#include "process.hpp"

#if __linux__
#include "platform/linux/process.hpp"
#else
#endif

namespace utils {

process::process(cli_arguments const& args, std::string const& folder) : impl_(nullptr) {
    impl_ = new process_platform_impl(args, folder);
}
process::~process() {
    // no wait process
    if (impl_)
        delete impl_;
}

void process::launch() { impl_->launch(); }
void process::terminate() { impl_->terminate(); }
int64_t process::wait() { return impl_->wait(); }
int64_t process::pid() { return impl_->pid(); }
}
