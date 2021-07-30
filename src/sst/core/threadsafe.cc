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

#include "sst/core/profile.h"
#include "sstpause.h"
#include "threadsafe.h"

#include <thread>


double SST::Core::ThreadSafe::Barrier::wait(){
  double elapsed = 0.0;
  if ( enabled ) {
      auto startTime = SST::Core::Profile::now();

      size_t gen = generation.load(std::memory_order_acquire);
      asm("" ::: "memory");
      size_t c = count.fetch_sub(1) - 1;
      if ( 0 == c ) {
          /* We should release */
          count.store(origCount);
          asm("" ::: "memory");
          /* Incrementing generation causes release */
          generation.fetch_add(1, std::memory_order_release);
          __sync_synchronize();
      }
      else {
          /* Try spinning first */
          uint32_t count = 0;
          do {
              count++;
              if ( count < 1024 ) { sst_pause(); }
              else if ( count < (1024 * 1024) ) {
                  std::this_thread::yield();
              }
              else {
                  struct timespec ts;
                  ts.tv_sec  = 0;
                  ts.tv_nsec = 1000;
                  nanosleep(&ts, nullptr);
              }
          } while ( gen == generation.load(std::memory_order_acquire) );
      }
      elapsed = SST::Core::Profile::getElapsed(startTime);
  }
  return elapsed;
}

void SST::Core::ThreadSafe::Spinlock::lock()
    {
        while ( latch.test_and_set(std::memory_order_acquire) ) {
            sst_pause();
#if defined(__PPC64__)
            __sync_synchronize();
#endif
        }
    }

void SST::Core::ThreadSafe::Spinlock::unlock(){ latch.clear(std::memory_order_release); }
