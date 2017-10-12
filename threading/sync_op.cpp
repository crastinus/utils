#include "sync_op.hpp"
#include "sync_op_impl.hpp"
#include <thread>

namespace concurrent {
    sync_op::sync_op(size_t workers_count) : impl_(nullptr) {
    if (workers_count == 0)
        workers_count = static_cast<size_t>(std::thread::hardware_concurrency()) * 2;

    impl_ = new sync_op_impl(workers_count);
}

sync_op::~sync_op() {
    if (impl_ != nullptr)
        delete impl_;
}

void sync_op::post_task(std::function<void()> const& task) {
    impl_->post_task(task);
}

}
