/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010  The Microgrid Project.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
#ifndef MEMSSTORESEQUENCE_H
#define MEMSSTORESEQUENCE_H

#include <cstdlib>
#include <fstream>
#include <cstddef>
#include <stdint.h>

using namespace std;

namespace Simulator
{
    typedef struct __storeentryheader_t{
        uint64_t            cycleno;
        unsigned int        pid;
        uint64_t            address;
        unsigned int        size;
        uint64_t            cycleno_end;
    } storeentryheader_t;


    extern ofstream g_osSSDTraceFile;
    extern ofstream g_osSSRDTraceFile;

    void startSSRDebugtracing(char* filename);
    void startSSDebugtracing(char* filename);
    void stopSSRDebugtracing();
    void stopSSDebugtracing();
    // debug and trace store seqence 
    void debugSSproc(uint64_t cycleno, unsigned int pid, uint64_t address, unsigned int size, char* data);

    // debug and trace store sequence upon their return
    void debugSSRproc(uint64_t cycleno_start, uint64_t cycleno_end, unsigned int pid, uint64_t address, unsigned int size, char* data);
}


#endif
