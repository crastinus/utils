#pragma once

#include "sync_op.hpp"
#include "third_party/blockingconcurrentqueue.h"
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

namespace concurrent {

struct sync_op_impl {
    // multiproducer multiconsumer queue
    using blocking_multy_queue_t = moodycamel::BlockingConcurrentQueue<task_t>;

    sync_op_impl(size_t workers_count);
    ~sync_op_impl();

    void post_task(std::function<void()> const& task);

   private:
    // main loop for process
    void process_tasks();

    // flag for running
    bool running_;
    // tasks queue
    blocking_multy_queue_t queue_;
    // thread for queue
    std::thread thread_;
    };

}
