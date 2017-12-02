#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utils/string/build.hpp>

#include <thread>

namespace utils {
namespace log {

enum log_level {
    LOG_LEVEL_FATAL    = 0,
    LOG_LEVEL_CRITICAL = 1,
    LOG_LEVEL_ERROR    = 2,
    LOG_LEVEL_WARNING  = 3,
    LOG_LEVEL_INFO     = 4,
    LOG_LEVEL_VERBOSE  = 5,
    LOG_LEVEL_DEBUG    = 6,
    LOG_LEVEL_TRACE    = 7
};


struct internal_message {
    log_level   level_;
    std::string thread_name_;
    std::string message_;
    uint64_t    timestamp_;
};

// Maybe is worth to avoid this class. Moodycamel queue has such a behaviour as if caches the messages in thread local.
struct thread_local_instance {

    static thread_local_instance& get();

    thread_local_instance();
    ~thread_local_instance();

    // timeout == max ? flush outside write : flush by timeout 
    void set_flush_timeout(uint64_t timeout);
    void set_flush_count(uint64_t count);
    void set_name(std::string const& thread_name);
    std::string const& thread_name() const;

    void write(log_level level, std::string message);
    void flush();

private:

    // flush condition. If all of this values is 0 then do flush by using flush method in your code
    uint64_t                        flush_period_in_ms_;
    uint64_t                        last_flush_timestamp_in_ms_;
    uint64_t                        max_count_;

    std::string                     thread_name_;
    std::vector<internal_message>   cache_;
};

void init(log_level level, std::string const& path);
void destroy();

void set_thread_name(std::string const& name);
void set_thread_flush_timeout(uint64_t timeout_in_ms = 0);
void set_thread_flush_count(uint64_t count = 0);
void flush_thread_local_buffer();

log_level loglvl();

template<typename ...Args>
void write(log_level level, Args&& ... args) {

    if (!(loglvl() <= level))
        return;

    thread_local_instance::get().write(level, utils::build_string(std::forward<Args>(args)...));
}

}
} // namespace utils

namespace logs {
    template<typename ...Args> inline void fatal(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_FATAL, std::forward<Args>(args)...); }
    template<typename ...Args> inline void critical(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_CRITICAL, std::forward<Args>(args)...); }
    template<typename ...Args> inline void error(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_ERROR, std::forward<Args>(args)...); }
    template<typename ...Args> inline void warning(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_WARNING, std::forward<Args>(args)...); }
    template<typename ...Args> inline void info(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_INFO, std::forward<Args>(args)...); }
    template<typename ...Args> inline void verbose(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_VERBOSE, std::forward<Args>(args)...); }
    template<typename ...Args> inline void debug(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_DEBUG, std::forward<Args>(args)...); }
    template<typename ...Args> inline void trace(Args&& ...args) { utils::log::write(utils::log::LOG_LEVEL_TRACE, std::forward<Args>(args)...); }
}