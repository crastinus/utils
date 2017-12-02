#pragma once

#ifdef ATOMIC_QUEUE
#include "third_party/blockingconcurrentqueue.h"
#else
#include "block_queue_lock.hpp"
#endif

namespace utils {


#ifdef ATOMIC_QUEUE
template <typename ElementType>
using block_queue = moodycamel::BlockingConcurrentQueue<ElementType>;
#else
template <typename ElementType>
using block_queue = block_queue_lock<ElementType>;
#endif

}
