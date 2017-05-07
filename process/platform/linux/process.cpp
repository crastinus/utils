#include "process.hpp"

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


namespace utils {
process_platform_impl::process_platform_impl(cli_arguments const& args, std::string const& folder)
    : arguments_(args), folder_(folder), pid_(0) {}

process_platform_impl::~process_platform_impl() {}

void process_platform_impl::launch() {

    if (unshare(CLONE_FILES) == -1)
        return;

    pid_ = fork();
    if (pid_ != 0)
        return;

    if (!folder_.empty())
        chdir(folder_.data());

    auto args = arguments_.argv();
    
    //TODO: Error handling
    execvp(args[0],args.data());
    

}
void process_platform_impl::terminate() {
    if (pid_ == 0)
        return;
    kill(pid_, SIGKILL);
    wait();
}

int64_t process_platform_impl::wait() {
    if (pid_ == 0)
        return 0;

    int status;
    waitpid(pid_, &status, 0);

    pid_ = 0;
    return status;
}

int64_t process_platform_impl::pid() {
    return pid_;
}

}
