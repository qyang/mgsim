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
    virtual bool OnMemoryReadCompleted(const MemData& data) = 0;
    virtual bool OnMemoryWriteCompleted(const MemTag& tag) = 0;
    virtual bool OnMemorySnooped(MemAddr /* addr */, const MemData& /* data */) { return true; }

    virtual ~IMemoryCallback() {}
};

class IMemory
{
public:
	static const int PERM_EXECUTE = 1;
	static const int PERM_WRITE   = 2;
	static const int PERM_READ    = 4;

    virtual void   Reserve(MemAddr address, MemSize size, int perm) = 0;
    virtual void   Unreserve(MemAddr address) = 0;
    virtual void   RegisterListener  (IMemoryCallback& callback) = 0;
    virtual void   UnregisterListener(IMemoryCallback& callback) = 0;
    virtual Result Read (IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag) = 0;
    virtual Result Write(IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag) = 0;
	virtual bool   CheckPermissions(MemAddr address, MemSize size, int access) const = 0;

    virtual ~IMemory() {}
};

class IMemoryAdmin : public IMemory
{
public:
    virtual bool Allocate(MemSize size, int perm, MemAddr& address) = 0;
    virtual void Read (MemAddr address, void* data, MemSize size) = 0;
    virtual void Write(MemAddr address, const void* data, MemSize size) = 0;

    virtual ~IMemoryAdmin() {}
};

}
#endif

