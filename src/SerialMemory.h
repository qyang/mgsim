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
#ifndef SERIALMEMORY_H
#define SERIALMEMORY_H

#include "Memory.h"
#include "storage.h"
#include "VirtualMemory.h"
#include <deque>
#include <set>

class Config;

namespace Simulator
{

class SerialMemory : public Object, public IMemoryAdmin, public VirtualMemory
{
    struct Request
    {
        bool                write;
        MemAddr             address;
        MemData             data;
        IMemoryCallback*    callback;
    };

    // IMemory
    void Reserve(MemAddr address, MemSize size, int perm);
    void Unreserve(MemAddr address);
    void RegisterClient  (PSize pid, IMemoryCallback& callback, const Process* processes[]);
    void UnregisterClient(PSize pid);
    bool Read (PSize pid, MemAddr address, MemSize size, MemTag tag);
    bool Write(PSize pid, MemAddr address, const void* data, MemSize size, MemTag tag);
	bool CheckPermissions(MemAddr address, MemSize size, int access) const;

    // IMemoryAdmin
    bool Allocate(MemSize size, int perm, MemAddr& address);
    void Read (MemAddr address, void* data, MemSize size);
    void Write(MemAddr address, const void* data, MemSize size);

    void GetMemoryStatistics(uint64_t& nreads, uint64_t& nwrites, 
                             uint64_t& nread_bytes, uint64_t& nwrite_bytes) const
    {
        nreads = m_nreads;
        nread_bytes = m_nread_bytes;
        nwrites = m_nwrites;
        nwrite_bytes = m_nwrite_bytes;
    }

    std::vector<IMemoryCallback*> m_clients;
    Buffer<Request>               m_requests;
    ArbitratedService             p_requests;
    CycleNo                       m_baseRequestTime;
    CycleNo                       m_timePerLine;
    CycleNo                       m_sizeOfLine;
    CycleNo                       m_nextdone;

    uint64_t m_nreads;
    uint64_t m_nread_bytes;
    uint64_t m_nwrites;
    uint64_t m_nwrite_bytes;
    
    // Processes
    Process p_Requests;
    
    Result DoRequests();
    
public:
    SerialMemory(const std::string& name, Object& parent, const Config& config);

    // Debugging
    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif

