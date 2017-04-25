#pragma once

// for debug in lldb
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace utils {
    template <typename T>
    class async_queue {
    public:

        T dequeue() {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                cond_.wait(mlock);
            }
            auto item = queue_.front();
            queue_.pop();
            return item;
        }

        void dequeue(T& item) {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                cond_.wait(mlock);
            }
            item = std::move(queue_.front());
            queue_.pop();
        }

        void dequeue_array(std::queue<T>& newqueue){
            
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty()) {
                cond_.wait(mlock);
            }
            std::swap(newqueue, queue_);          
        }

        template <typename It>
        void wait_dequeue_bulk(It it, unsigned) {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty()) {
                cond_.wait(mlock);
            }

            while (!queue_.empty()) {
                it = queue_.pop();
                ++it;
            }
        }

        void enqueue(const T& item) {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.enqueue(item);
            mlock.unlock();
            cond_.notify_one();
        }

        void enqueue(T&& item) {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.enqueue(std::move(item));
            mlock.unlock();
            cond_.notify_one();
        }

        bool empty() const{
            return queue_.empty();
        }

    private:
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}
