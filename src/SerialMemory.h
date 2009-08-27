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

class SerialMemory : public IComponent, public IMemoryAdmin, public VirtualMemory
{
    struct Request
    {
        bool                write;
        MemAddr             address;
        MemData             data;
        IMemoryCallback*    callback;
    };

    // Component
    Result OnCycle(unsigned int stateIndex);

    // IMemory
    void Reserve(MemAddr address, MemSize size, int perm);
    void Unreserve(MemAddr address);
    void RegisterListener  (PSize pid, IMemoryCallback& callback, const ArbitrationSource* sources);
    void UnregisterListener(PSize pid, IMemoryCallback& callback);
    bool Read (IMemoryCallback& callback, MemAddr address, MemSize size, MemTag tag);
    bool Write(IMemoryCallback& callback, MemAddr address, const void* data, MemSize size, MemTag tag);
	bool CheckPermissions(MemAddr address, MemSize size, int access) const;

    // IMemoryAdmin
    bool Allocate(MemSize size, int perm, MemAddr& address);
    void Read (MemAddr address, void* data, MemSize size);
    void Write(MemAddr address, const void* data, MemSize size);

    std::set<IMemoryCallback*>  m_caches;
    Buffer<Request>             m_requests;
    ArbitratedService           p_requests;
    CycleNo                     m_baseRequestTime;
    CycleNo                     m_timePerLine;
    CycleNo                     m_sizeOfLine;
    CycleNo                     m_nextdone;

public:
    SerialMemory(Object* parent, Kernel& kernel, const std::string& name, const Config& config);

    // Debugging
    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif

