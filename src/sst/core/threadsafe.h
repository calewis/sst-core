// Copyright 2009-2021 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2021, NTESS
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef SST_CORE_THREADSAFE_H
#define SST_CORE_THREADSAFE_H


#include <atomic>
// #include <condition_variable>
// #include <mutex>
// #include <vector>
//#include <stdalign.h>

// #include "sst/core/profile.h"

#include <time.h>

namespace SST {
namespace Core {
namespace ThreadSafe {

#if defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ < 8))
#define CACHE_ALIGNED(type, name) type name __attribute__((aligned(64)))
#define CACHE_ALIGNED_T
#else
#define CACHE_ALIGNED(type, name) alignas(64) type name
#define CACHE_ALIGNED_T           alignas(64)
#endif

class CACHE_ALIGNED_T Barrier
{
    size_t              origCount;
    std::atomic<bool>   enabled;
    std::atomic<size_t> count, generation;

public:
    Barrier(size_t count) : origCount(count), enabled(true), count(count), generation(0) {}

    // Come g++ 4.7, this can become a delegating constructor
    Barrier() : origCount(0), enabled(false), count(0), generation(0) {}

    /** ONLY call this while nobody is in wait() */
    void resize(size_t newCount)
    {
        count = origCount = newCount;
        generation.store(0);
        enabled.store(true);
    }

    /**
     * Wait for all threads to reach this point.
     * @return 0.0, or elapsed time spent waiting, if configured with --enable-profile
     */
    double wait();
    void disable()
    {
        enabled.store(false);
        count.store(0);
        ++generation;
    }
};

#if 0
typedef std::mutex Spinlock;
#else
class Spinlock
{
    std::atomic_flag latch = ATOMIC_FLAG_INIT;

public:
    Spinlock() {}

    void lock();
    void unlock();
};
#endif

} // namespace ThreadSafe
} // namespace Core
} // namespace SST

#endif // SST_CORE_THREADSAFE_H
