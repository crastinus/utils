#pragma once

#include <functional>
#include <thread>
#include <assert.h>

namespace concurrent {


using task_t = std::function<void()>;

struct sync_op_impl;
struct sync_op {
    sync_op(size_t workers_count = 0);
    ~sync_op();

    void post_task(task_t const& task);

   private:
    sync_op_impl* impl_;
};
}

#define REQUIRE_READ_THREAD \
    assert(concurrent::thread_type() == concurrent::thread_type_enum::READ_THREAD);

#define REQUIRE_MODIFY_THREAD \
    assert(concurrent::thread_type() == concurrent::thread_type_enum::MODIFY_THREAD);
