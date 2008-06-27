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
#ifndef BANKEDMEMORY_H
#define BANKEDMEMORY_H

#include <queue>
#include <set>
#include "Memory.h"
#include "kernel.h"
#include "VirtualMemory.h"

namespace Simulator
{

class ArbitratedWriteFunction;

class BankedMemory : public IComponent, public IMemory, public IMemoryAdmin, public VirtualMemory
{
public:
	struct Config
	{
        CycleNo    baseRequestTime;
        CycleNo    timePerLine;
        size_t     sizeOfLine;
        size_t     bufferSize;
        size_t     numBanks;
	};

    class Request
    {
        void release();

    public:
        unsigned long*   refcount;
        IMemoryCallback* callback;
        bool             write;
        MemAddr          address;
        MemData          data;

        Request& operator =(const Request& req);
        Request(const Request& req);
        Request();
        ~Request();
    };
    
    struct Bank
    {
        ArbitratedWriteFunction* func;
        bool                     busy;
        Request                  request;
        CycleNo                  done;
        
        Bank();
        ~Bank();
    };

    BankedMemory(Object* parent, Kernel& kernel, const std::string& name, const Config& config, size_t nProcs);
    
    typedef std::multimap<CycleNo, Request> Pipeline;
   
    const std::vector<Bank>&     GetBanks()    const { return m_banks; }
    const std::vector<Pipeline>& GetIncoming() const { return m_incoming; }
    const std::vector<Pipeline>& GetOutgoing() const { return m_outgoing; }

    size_t GetQueueIndex(IMemoryCallback* callback);
    size_t GetBankFromAddress(MemAddr address) const;
    
    // Component
    Result onCycleWritePhase(unsigned int stateIndex);

    // IMemory
    void registerListener  (IMemoryCallback& callback);
    void unregisterListener(IMemoryCallback& callback);
    Result read (IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag);
    Result write(IMemoryCallback& callback, MemAddr address, void* data, MemSize size, MemTag tag);
	bool checkPermissions(MemAddr address, MemSize size, int access) const;

    // IMemoryAdmin
    void read (MemAddr address, void* data, MemSize size);
    void write(MemAddr address, const void* data, MemSize size, int perm = 0);

private:
    Config                      m_config;
    std::set<IMemoryCallback*>  m_caches;
    std::vector<Bank>           m_banks;
    
    void AddRequest(Pipeline& queue, const Request& request, bool data);
    
    std::vector<Pipeline> m_incoming;
    std::vector<Pipeline> m_outgoing;
    std::map<IMemoryCallback*, size_t> m_portmap;
};

}
#endif

