// Copyright 2009-2010 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2010, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#include "sst_config.h"

#include <boost/mpi.hpp>

#include "sst/core/clockEvent.h"
#include "sst/core/stopEvent.h"
#include "sst/core/syncEvent.h"
#include "sst/core/compEvent.h"
#include "sst/core/exitEvent.h"

#if WANT_CHECKPOINT_SUPPORT
//BOOST_CLASS_EXPORT( SST::ClockEvent )
BOOST_CLASS_EXPORT( SST::StopEvent )
BOOST_CLASS_EXPORT( SST::SyncEvent )
BOOST_CLASS_EXPORT( SST::ExitEvent )

BOOST_CLASS_EXPORT_TEMPLATE3( SST::EventHandler,
                                SST::StopEvent, bool, SST::Event* )

#endif
    
BOOST_CLASS_EXPORT( SST::CompEvent )
BOOST_CLASS_EXPORT( SST::Event )

#if 0
BOOST_IS_MPI_DATATYPE( SST::CompEvent )
BOOST_IS_MPI_DATATYPE( SST::Event )
#endif
