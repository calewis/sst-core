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

#ifndef SST_CORE_SSTPAUSE_H
#define SST_CORE_SSTPAUSE_H

#if ( defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__) )
#include <emmintrin.h>
#define sst_pause() _mm_pause()
#elif (defined(__arm__) || defined(__arm) || defined(__aarch64__))
#define sst_pause() __asm__ __volatile__("yield")
#elif defined(__PPC64__)
#define sst_pause() __asm__ __volatile__("or 27, 27, 27" ::: "memory");
#endif

#endif // SST_CORE_SSTPAUSE_H
