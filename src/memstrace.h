/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009  The Microgrid Project.

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
#ifndef MEMSTRACE_H
#define MEMSTRACE_H

#include <cstdlib>
#include <fstream>
#include <cstddef>

using namespace std;

#include "VirtualMemory.h"
#include "kernel.h"
#include "coma/memorys/dcswitch.h"

#ifdef MEM_STORE_SEQUENCE_DEBUG
#include "memsstoresequence.h"
#endif

namespace Simulator
{

extern ofstream g_osTraceFile;
extern size_t g_pid;
extern size_t g_tid;
extern ifstream g_isBatchFile;
extern ofstream g_osTraceFileDCache;
extern bool g_bEnableTraceDCache;
extern uint64_t g_u64TraceAddress;


void starttracing(const char* filename);
void stoptracing();
void tracepid(size_t pid);
void tracetid(size_t tid);
void traceproc(uint64_t cycleno, size_t pid, size_t tid, bool bwrite, uint64_t addr, size_t size, char* data);
void settraceaddress(uint64_t addr);
void tracecacheproc(Object* obj, uint64_t cycleno, size_t pid, unsigned int ext=0);

}

#endif
