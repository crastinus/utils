#pragma once
#include <utils/process/cli_arguments.hpp>

namespace utils {
    
    struct process_platform_impl {

        process_platform_impl(cli_arguments const& args, std::string const& folder = "");
        ~process_platform_impl();

        void launch();
        void terminate();
        int64_t wait();
        int64_t pid();

      private:
        //cli_arguments arguments_;
        //std::string folder_;
    };
}
 

