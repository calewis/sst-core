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


#include "categoryInfo.h"
#include <ostream>

void SST::ELI::ProvidesCategory::toString(std::ostream& UNUSED(os)) const { os << "      CATEGORY: " << categoryName(cat_) << "\n"; }
