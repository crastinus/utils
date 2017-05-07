#pragma once

#include <string>
#include "cli_arguments.hpp"

namespace utils {

struct process_platform_impl;
struct process {

    process(cli_arguments const& args, std::string const& folder = "");
    ~process();

    void launch();
    void terminate();
    int64_t wait();
    int64_t pid();

  private:
    process_platform_impl* impl_;
};
}
