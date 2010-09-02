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
#ifndef CACHELINK_H
#define CACHELINE_H

#include <queue>
#include <set>
#include "Memory.h"
#include "kernel.h"
#include "config.h"
#include "coma/simlink/linkmgs.h"
#include "Processor.h"

namespace Simulator
{

class CMLink : public Object, public IMemoryAdmin
{
public:
    struct Request
    {
        bool                write;
        CycleNo             done;
        MemAddr             address;
        MemData             data;
        TID                 tid;
        IMemoryCallback*    callback;
        CycleNo             starttime;  // MESMX debug
        bool                bconflict;   
    };
        
    CMLink(const std::string& name, Object& parent, Clock& clock, const Config& config, LinkMGS* linkmgs);

    void RegisterClient(PSize pid, IMemoryCallback& callback, const Process* processes[]);
    void UnregisterClient(PSize pid);

    void Reserve(MemAddr address, MemSize size, int perm);
    void Unreserve(MemAddr address);

    bool Read (PSize pid, MemAddr address, MemSize size);
    bool Write(PSize pid, MemAddr address, const void* data, MemSize size, TID tid);

    // IMemory
	bool CheckPermissions(MemAddr address, MemSize size, int access) const;
	
	Process p_Requests;

    Result DoRequests();

    // IMemoryAdmin
    bool Allocate(MemSize size, int perm, MemAddr& address);
    void Read (MemAddr address, void* data, MemSize size);
    void Write(MemAddr address, const void* data, MemSize size);

    void GetMemoryStatistics(uint64_t&, uint64_t&, uint64_t&, uint64_t&) const;
    void SetProcessor(Processor* proc){m_pProcessor = proc;}
    Processor* GetProcessor(){return m_pProcessor;}

private:
    Buffer<Request>               m_requests;
    std::vector<IMemoryCallback*> m_clients;
    std::set<Request*>            m_setrequests;     // MESMX 
    unsigned int                  m_nTotalReq;
    LinkMGS*                      m_linkmgs;     // MESMX
    IMemoryCallback*              m_pimcallback;
    Processor*                    m_pProcessor;
};

}
#endif
