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
#ifndef MEMORY_H
#define MEMORY_H

#include "simtypes.h"

namespace Simulator
{

struct MemData
{
    MemTag  tag;
    void*   data;
    MemSize size;
};

class IMemory;

class IMemoryCallback
{
public:
    virtual bool onMemoryReadCompleted(const MemData& data) = 0;
    virtual bool onMemoryWriteCompleted(const MemTag& tag) = 0;
    virtual bool onMemorySnooped(MemAddr addr, const MemData& data) { return true; }

    virtual ~IMemoryCallback() {}
};

class IMemory
{
public:
    virtual void registerListener  (IMemoryCallback& callback) = 0;
    virtual void unregisterListener(IMemoryCallback& callback) = 0;
    virtual Result read (IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag) = 0;
    virtual Result write(IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag) = 0;

    virtual ~IMemory() {}
};

class IMemoryAdmin
{
public:
    virtual void read (MemAddr address, void* data, MemSize size) = 0;
    virtual void write(MemAddr address, void* data, MemSize size) = 0;

    virtual ~IMemoryAdmin() {}
};

}
#endif

