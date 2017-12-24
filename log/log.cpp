#include "log.hpp"

#include <utils/threading/third_party/blockingconcurrentqueue.h>
#include <utils/time/time.hpp>
#include <utils/string/build.hpp>
#include <utils/filesystem/filesystem.hpp>
#include <atomic>
#include <iostream>
#include <iomanip>
#include <fstream>

namespace utils {
namespace log {


struct global_instance {

    static std::unique_ptr<global_instance> instance_;
    
    using queue_t = moodycamel::BlockingConcurrentQueue<internal_message>;

    global_instance(std::string const& path2folder, log_level level);
    ~global_instance();

    void loop();
    void push(internal_message&& value);
    void push(std::vector<internal_message>& value);

    log_level level() const { return level_; }

 private:

     void write_logs(std::vector<internal_message> const& value);

     /*
        \brief extracts logs from queue and writes it into a file
        \param logs_buffer - temprorary preallocated buffer
        \param count - max logs count
     */
     void try_write_logs_internal(std::vector<internal_message>& logs_buffer, size_t count);

    std::string path_to_folder_;
    log_level   level_;

    queue_t queue_;

    bool cancelation_token_;
    std::thread thread_;
};

std::unique_ptr<global_instance> global_instance::instance_;

/*
    Manipulator for helping name output
    20181011.log
*/
struct log_filename {
    uint64_t timestamp_;
    log_filename(uint64_t initial_timestamp)
        : timestamp_(initial_timestamp) {}
};

std::ostream& operator<<(std::ostream& os, log_filename const& lfilename) {

    time_t    t = static_cast<time_t>(lfilename.timestamp_) / 1000;
    struct tm time;
    ::localtime_s(&time, &t);

    os << time.tm_year + 1900 << std::setw(2) << std::setfill('0') << time.tm_mon + 1
       << std::setw(2) << std::setfill('0') << time.tm_mday << ".log";

    return os;
}

/*
    Manipulator for writing timestamp in log line
    [2017-01-23 10:28:01.456]
*/
struct log_timestamp {
    uint64_t timestamp_;
    log_timestamp(uint64_t initial_timestamp)
        : timestamp_(initial_timestamp) {}
};

std::ostream& operator<<(std::ostream& os, log_timestamp const& ltimestamp) {

    time_t    t = static_cast<time_t>(ltimestamp.timestamp_) / 1000;
    struct tm time;
    ::localtime_s(&time, &t);

    os << '[' << time.tm_year + 1900 << '-' 
       << std::setw(2) << std::setfill('0') << time.tm_mon + 1 << '-'
       << std::setw(2) << std::setfill('0') << time.tm_mday << ' ' 
       << std::setw(2) << std::setfill('0') << time.tm_hour << ':' 
       << std::setw(2) << std::setfill('0') << time.tm_min  << ':'
       << std::setw(2) << std::setfill('0') << time.tm_sec << '.' 
       << std::setw(3) << std::setfill('0') << (ltimestamp.timestamp_ % 1000) << ']';

    return os;
}

global_instance::global_instance(std::string const& path, log_level level)
    : path_to_folder_(path)
    , level_(level)
    , cancelation_token_(true)
    , thread_(&global_instance::loop, this) {}

global_instance::~global_instance() {

    cancelation_token_ = false;
    push(internal_message{ log_level::LOG_LEVEL_ERROR, thread_local_instance::get().thread_name(),
                           "Stop log thread", utils::time::timestamp_in_ms() });

    if (thread_.joinable())
        thread_.join();

    try {
        std::vector<internal_message> buffer;
        queue_.try_dequeue_bulk(std::back_inserter(buffer), std::numeric_limits<size_t>::max() / 2);
        if (!buffer.empty()) {
            write_logs(buffer);
        }
    }
    catch (...) {}
}

void global_instance::loop() {
    
    std::vector<internal_message> buffer;
    buffer.reserve(1024);
    
    while (cancelation_token_) {
        try_write_logs_internal(buffer, buffer.capacity());
        buffer.clear();
    }
}

void global_instance::push(internal_message&& value) {
    queue_.enqueue(std::move(value));
}

void global_instance::push(std::vector<internal_message>& value) {
    queue_.enqueue_bulk(std::move_iterator<decltype(value.begin())>(value.begin()), value.size());
}

static char log_level_map[] = { 'F' , 'C' , 'E' , 'W' , 'I' , 'V' , 'D' , 'T' };
void global_instance::write_logs(std::vector<internal_message> const& logs) {

    assert(!logs.empty());
    if (logs.empty())
        return;
    
    // open a file for writing
    auto path = utils::build_string(path_to_folder_, "/", log_filename(logs.front().timestamp_));
    if (!utils::fs::exists(path)) {
        utils::fs::mkdir_for_file(path);
    }

    std::ofstream out;
    out.exceptions(std::ios::badbit | std::ios::failbit);
    out.open(path);

    for (auto const& log : logs) {
        
        assert(log.level_ <= log_level::LOG_LEVEL_TRACE);

        out << log_timestamp(log.timestamp_) << ' ' << log_level_map[static_cast<unsigned>(log.level_)]
            << " [" << log.thread_name_ << "] " << log.message_ << '\n';
    }
}

void global_instance::try_write_logs_internal(std::vector<internal_message>& logs_buffer, size_t count) {
    try {
        queue_.wait_dequeue_bulk(std::back_inserter(logs_buffer), count);
        write_logs(logs_buffer);
    } catch (std::exception& ex) {
        std::cerr << "Cannot write logs. Error: " << ex.what() << "\n";
    } catch (...) {
        std::cerr << "Cannot write logs. Unknown error\n";
    }
}


/*
    Thread local instance
*/
thread_local thread_local_instance tl_instance;
thread_local_instance&             thread_local_instance::get() {
    return tl_instance;
}

thread_local_instance::thread_local_instance() {
    last_flush_timestamp_in_ms_ = ::utils::time::timestamp_in_ms();
}

thread_local_instance::~thread_local_instance() {}

void thread_local_instance::set_flush_timeout(uint64_t timeout) {
    flush_period_in_ms_ = timeout;
}

void thread_local_instance::set_flush_count(uint64_t count) {
    max_count_ = count;
}

void thread_local_instance::set_name(std::string const& thread_name) {
    thread_name_ = thread_name;
}

std::string const& thread_local_instance::thread_name() const {
    return thread_name_;
}

void thread_local_instance::write(log_level level, std::string message) {
    cache_.emplace_back();
    internal_message& result = cache_.back();

    result.message_     = std::move(message);
    result.level_       = level;
    result.timestamp_   = utils::time::timestamp_in_ms();
    result.thread_name_ = thread_name_;

    // checkout for cache flush
    bool is_timeout_reached = ((flush_period_in_ms_ != 0) &&
                               (last_flush_timestamp_in_ms_ - result.timestamp_) > flush_period_in_ms_);
    bool is_size_reached    = ((max_count_ != 0) && (cache_.size() >= max_count_));

    if (is_timeout_reached || is_size_reached) {
        flush();
    }
}

void thread_local_instance::flush() {

    if (cache_.empty())
        return;

    // update timestamp
    last_flush_timestamp_in_ms_ = cache_.back().timestamp_;

    // send to another
    assert(global_instance::instance_);
    global_instance::instance_->push(cache_);
    cache_.clear();
}


void init(log_level level, std::string const& path) {
    global_instance::instance_ = std::make_unique<global_instance>(path, level);
}

void destroy() {
    global_instance::instance_.reset();
}

void set_thread_name(std::string const& name) {
    thread_local_instance::get().set_name(name);
}

void set_thread_flush_timeout(uint64_t timeout) {
    thread_local_instance::get().set_flush_timeout(timeout);
}

void set_thread_flush_count(uint64_t count) {
    thread_local_instance::get().set_flush_count(count);
}

void flush_thread_local_buffer() {
    thread_local_instance::get().flush();
}

log_level loglvl()
{
    assert(global_instance::instance_);
    return global_instance::instance_->level();
}

} // namespace log
} // namespace utils