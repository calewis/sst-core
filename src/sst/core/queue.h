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

#ifndef SST_CORE_QUEUE_H
#define SST_CORE_QUEUE_H

#include "sst/core/threadsafe.h"
#include "sst/core/sstpause.h"

#include <atomic>
#include <mutex>

namespace SST {
namespace Core {
namespace ThreadSafe {

template <typename T>
class BoundedQueue
{

    struct cell_t
    {
        std::atomic<size_t> sequence;
        T                   data;
    };

    bool    initialized;
    size_t  dsize;
    cell_t* data;
    CACHE_ALIGNED(std::atomic<size_t>, rPtr);
    CACHE_ALIGNED(std::atomic<size_t>, wPtr);

public:
    // BoundedQueue(size_t maxSize) : dsize(maxSize)
    BoundedQueue(size_t maxSize) : initialized(false) { initialize(maxSize); }

    BoundedQueue() : initialized(false) {}

    void initialize(size_t maxSize)
    {
        if ( initialized ) return;
        dsize = maxSize;
        data  = new cell_t[dsize];
        for ( size_t i = 0; i < maxSize; i++ )
            data[i].sequence.store(i);
        rPtr.store(0);
        wPtr.store(0);
        // fprintf(stderr, "%p  %p:  %ld\n", &rPtr, &wPtr, ((intptr_t)&wPtr - (intptr_t)&rPtr));
        initialized = true;
    }

    ~BoundedQueue()
    {
        if ( initialized ) delete[] data;
    }

    size_t size() const { return (wPtr.load() - rPtr.load()); }

    bool empty() const { return (rPtr.load() == wPtr.load()); }

    bool try_insert(const T& arg)
    {
        cell_t* cell = nullptr;
        size_t  pos  = wPtr.load(std::memory_order_relaxed);
        for ( ;; ) {
            cell          = &data[pos % dsize];
            size_t   seq  = cell->sequence.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)seq - (intptr_t)pos;
            if ( 0 == diff ) {
                if ( wPtr.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed) ) break;
            }
            else if ( 0 > diff ) {
                // fprintf(stderr, "diff = %ld: %zu - %zu\n", diff, seq, pos);
                return false;
            }
            else {
                pos = wPtr.load(std::memory_order_relaxed);
            }
        }
        cell->data = arg;
        cell->sequence.store(pos + 1, std::memory_order_release);
        return true;
    }

    bool try_remove(T& res)
    {
        cell_t* cell = nullptr;
        size_t  pos  = rPtr.load(std::memory_order_relaxed);
        for ( ;; ) {
            cell          = &data[pos % dsize];
            size_t   seq  = cell->sequence.load(std::memory_order_acquire);
            intptr_t diff = (intptr_t)seq - (intptr_t)(pos + 1);
            if ( 0 == diff ) {
                if ( rPtr.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed) ) break;
            }
            else if ( 0 > diff ) {
                return false;
            }
            else {
                pos = rPtr.load(std::memory_order_relaxed);
            }
        }
        res = cell->data;
        cell->sequence.store(pos + dsize, std::memory_order_release);
        return true;
    }

    T remove()
    {
        while ( 1 ) {
            T res;
            if ( try_remove(res) ) { return res; }
            sst_pause();
        }
    }
};

template <typename T>
class UnboundedQueue
{
    struct CACHE_ALIGNED_T Node
    {
        std::atomic<Node*> next;
        T                  data;

        Node() : next(nullptr) {}
    };

    CACHE_ALIGNED(Node*, first);
    CACHE_ALIGNED(Node*, last);
    CACHE_ALIGNED(Spinlock, consumerLock);
    CACHE_ALIGNED(Spinlock, producerLock);

public:
    UnboundedQueue()
    {
        /* 'first' is a dummy value */
        first = last = new Node();
    }

    ~UnboundedQueue()
    {
        while ( first != nullptr ) { // release the list
            Node* tmp = first;
            first     = tmp->next;
            delete tmp;
        }
    }

    void insert(const T& t)
    {
        Node* tmp = new Node();
        tmp->data = t;
        std::lock_guard<Spinlock> lock(producerLock);
        last->next = tmp; // publish to consumers
        last       = tmp; // swing last forward
    }

    bool try_remove(T& result)
    {
        std::lock_guard<Spinlock> lock(consumerLock);
        Node*                     theFirst = first;
        Node*                     theNext  = first->next;
        if ( theNext != nullptr ) { // if queue is nonempty
            result = theNext->data; // take it out
            first  = theNext;       // swing first forward
            delete theFirst;        // delete the old dummy
            return true;
        }
        return false;
    }

    T remove()
    {
        while ( 1 ) {
            T res;
            if ( try_remove(res) ) { return res; }
            sst_pause();
        }
    }
};

} // namespace ThreadSafe
} // namespace Core
} // namespace SST
#endif // SST_CORE_QUEUE_H
