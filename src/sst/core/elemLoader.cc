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

#include "sst_config.h"

#include "elemLoader.h"

#include "sst/core/component.h"
#include "sst/core/eli/elementinfo.h"
#include "sst/core/part/sstpart.h"
#include "sst/core/sstpart.h"
#include "sst/core/subcomponent.h"

#include <climits>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <vector>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

namespace SST {

namespace {
std::vector<std::string>
splitPath(std::string const& searchPaths)
{
    std::vector<std::string> paths;
    std::string::size_type   start = 0, pos = 0;
    while ( (pos = searchPaths.find(':', start)) != std::string::npos ) {
        paths.emplace_back(searchPaths.substr(start, pos - start));
        start = pos + 1;
    }
    paths.emplace_back(searchPaths.substr(start));

    return paths;
}
} // namespace

ElemLoader::ElemLoader(const std::string& searchPaths) :
    searchPaths(splitPath(searchPaths)),
    bindPolicy(RTLD_LAZY | RTLD_GLOBAL)
{
    const char* verbose_env = getenv("SST_CORE_DL_VERBOSE");
    if ( nullptr != verbose_env ) { verbose = atoi(verbose_env) > 0; }

    const char* bind_env = getenv("SST_CORE_DL_BIND_POLICY");
    if ( bind_env == nullptr ) { return; }

    if ( (!strcmp(bind_env, "now")) || (!strcmp(bind_env, "NOW")) ) { bindPolicy = RTLD_NOW | RTLD_GLOBAL; }
}

ElemLoader::~ElemLoader() = default;

void
ElemLoader::loadLibrary(const std::string& elemlib, std::ostream& err_os)
{
    std::stringstream pathSS;
    for ( std::string const& next_path : searchPaths ) {
        if ( verbose ) { printf("SST-DL: Searching: %s\n", next_path.c_str()); }

        if ( next_path.back() == '/' ) { pathSS << next_path << "lib" << elemlib << ".so"; }
        else {
            pathSS << next_path << "/lib" << elemlib << ".so";
        }

        auto full_path = pathSS.str();
        if ( verbose ) { printf("SST-DL: Attempting to load %s\n", full_path.c_str()); }

        // use a global bind policy read from environment, default to RTLD_LAZY
        void* handle = dlopen(full_path.c_str(), bindPolicy);

        if ( nullptr == handle ) {
            if ( verbose ) { printf("SST-DL: Loading failed, error: %s\n", dlerror()); }
        }
        else {
            if ( verbose ) { printf("SST-DL: Load was successful.\n"); }

            for ( auto& libpair : ELI::LoadedLibraries::getLoaders() ) {
                for ( auto& elempair : libpair.second ) {
                    for ( auto* loader : elempair.second ) {
                        loader->load();
                    }
                }
            }

            // exit the search loop, we have found the library we tried to load
            return;
        }
    }

    err_os << "Error: unable to find \"" << elemlib << "\" element library\n";
    return;
}

std::vector<std::string>
ElemLoader::getPotentialElements()
{
    std::vector<std::string> result;
    for ( std::string& next_path : searchPaths ) {
        DIR* current_dir = opendir(next_path.c_str());

        if ( nullptr == current_dir ) { continue; }

        struct dirent* dir_file;
        while ( (dir_file = readdir(current_dir)) != nullptr ) {
            // ensure we are only processing normal files and nothing weird
            if ( (dir_file->d_type | DT_REG) || (dir_file->d_type | DT_LNK) ) {
                std::string current_file = dir_file->d_name;

                // does the path start with "lib" required for SST
                if ( current_file.rfind("lib", 0) == 0 ) {
                    // find out if we have an extension
                    auto pos = current_file.find('.');
                    if ( pos != std::string::npos ) { result.push_back(current_file.substr(3, pos - 3)); }
                }
            }
        }

        closedir(current_dir);
    }

    return result;
}

} // namespace SST
