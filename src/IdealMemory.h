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
#ifndef IDEALMEMORY_H
#define IDEALMEMORY_H

#include "Memory.h"
#include "kernel.h"
#include "VirtualMemory.h"
#include <queue>
#include <set>

class Config;

namespace Simulator
{

class IdealMemory : public IComponent, public IMemoryAdmin, public VirtualMemory
{
public:
    class Request
    {
        void release()
        {
            if (refcount != NULL && --*refcount == 0) {
                delete[] (char*)data.data;
                delete refcount;
            }
        }

    public:
        unsigned long*      refcount;
        bool                write;
        CycleNo             done;
        MemAddr             address;
        MemData             data;
        IMemoryCallback*    callback;

        Request& operator =(const Request& req)
        {
            release();
            refcount  = req.refcount;
            write     = req.write;
            done      = req.done;
            address   = req.address;
            data      = req.data;
            callback  = req.callback;
            ++*refcount;
            return *this;
        }

        Request(const Request& req) : refcount(NULL) { *this = req; }
        Request() { refcount = new unsigned long(1); data.data = NULL; }
        ~Request() { release(); }
    };

    IdealMemory(Object* parent, Kernel& kernel, const std::string& name, const Config& config);

    CycleNo GetTotalWaitTime() const { return m_totalWaitTime; }

    // Component
    Result OnCycleWritePhase(unsigned int stateIndex);

    // IMemory
    void   Reserve(MemAddr address, MemSize size, int perm);
    void   Unreserve(MemAddr address);
    void   RegisterListener  (IMemoryCallback& callback);
    void   UnregisterListener(IMemoryCallback& callback);
    Result Read (IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag);
    Result Write(IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag);
	bool   CheckPermissions(MemAddr address, MemSize size, int access) const;

    // IMemoryAdmin
    bool Allocate(MemSize size, int perm, MemAddr& address);
    void Read (MemAddr address, void* data, MemSize size);
    void Write(MemAddr address, const void* data, MemSize size);

    const std::queue<Request>& GetRequests() const {
        return m_requests;
    }

private:
    std::set<IMemoryCallback*>  m_caches;
    std::queue<Request>         m_requests;
    BufferSize                  m_bufferSize;
    CycleNo                     m_baseRequestTime;
    CycleNo                     m_timePerLine;
    CycleNo                     m_sizeOfLine;
    CycleNo                     m_totalWaitTime;
};

}
#endif

