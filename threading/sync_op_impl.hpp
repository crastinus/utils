#pragma once

#include "sync_op.hpp"
#include <blockingconcurrentqueue.h>
#include <atomic>
#include <vector>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

namespace concurrent {

//struct block_queue {
//
//    void enqueue(task_t&& t) {
//        std::unique_lock<std::mutex> lock(mutex_);
//        tasks_q_.push(std::forward<task_t>(t));
//        lock.unlock();
//        cv_.notify_one();
//    }
//
//    void enqueue(task_t const& t) {
//        std::unique_lock<std::mutex> lock(mutex_);
//        tasks_q_.push(t);
//        lock.unlock();
//        cv_.notify_one();
//    }
//
//
//    template <typename It>
//    void wait_dequeue_bulk(It bi_it, unsigned count) {
//        std::unique_lock<std::mutex> lock(mutex_);
//        while (tasks_q_.empty())
//            cv_.wait(lock);
//
//        while (!tasks_q_.empty() && count != 0) {
//            bi_it = std::move(tasks_q_.front());
//            tasks_q_.pop();
//            ++bi_it;
//            --count;
//        }
//    }
//
//   private:
//    std::mutex                mutex_;
//    std::queue<task_t>        tasks_q_;
//    std::condition_variable cv_;
//};

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
