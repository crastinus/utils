#include "sync_op_impl.hpp"

#include <iostream>
#ifdef STDERR_TRACE
#include <iostream>
#endif

namespace concurrent {
sync_op_impl::sync_op_impl(size_t workers_count)
    : running_(true), thread_(&sync_op_impl::process_tasks, this) {}

sync_op_impl::~sync_op_impl() {
    running_ = false;
    post_task(task_t());
    if (thread_.joinable())
        thread_.join();
}

void sync_op_impl::post_task(task_t const& task) {
    queue_.enqueue(task);
}

void sync_op_impl::process_tasks() {
    //preallocated buffer size
    size_t constexpr tasks_buffer_size = 128;

    //init preallocated buffer
    std::vector<task_t> tasks_buffer;
    tasks_buffer.reserve(tasks_buffer_size);

    //executing loop
    while (running_) {
        queue_.wait_dequeue_bulk(std::back_inserter(tasks_buffer), tasks_buffer.capacity());

        for (auto& task : tasks_buffer)
            if (task)
                task();
        tasks_buffer.clear();
    }
}
}
