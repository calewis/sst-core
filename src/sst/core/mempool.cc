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


#include "mempool.h"
#include "sstpause.h"

#include <mutex>




namespace SST {
namespace Core {
        void MemPool::FreeList::insert(void* ptr)
        {
            std::lock_guard<ThreadSafe::Spinlock> lock(mtx);
            list.push_back(ptr);
        }

        void* MemPool::FreeList::try_remove()
        {
            std::lock_guard<ThreadSafe::Spinlock> lock(mtx);
            if ( list.empty() ) return nullptr;
            void* p = list.back();
            list.pop_back();
            return p;
        }

    void* MemPool::malloc()
    {
        void* ret = freeList.try_remove();
        while ( !ret ) {
            bool ok = allocPool();
            if ( !ok ) return nullptr;
            sst_pause();
            ret = freeList.try_remove();
        }
        ++numAlloc;
        return ret;
    }
}
}
