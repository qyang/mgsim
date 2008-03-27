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
#ifndef THREADTABLE_H
#define THREADTABLE_H

#include "ports.h"

namespace Simulator
{

class Processor;

struct Thread
{
    struct RegInfo
    {
        RegIndex base;
        RegIndex producer;
    };

    MemAddr     pc;
	FPCR        fpcr;
    RegInfo     regs[NUM_REG_TYPES];
    
    bool        killed;
    bool        nextKilled;
    bool        prevCleanedUp;
    bool        isLastThreadInBlock;
    bool        isFirstThreadInFamily;
    bool        isLastThreadInFamily;
    TID         prevInBlock;
    TID         nextInBlock;
    CID         cid;
    LFID        family;
    TID         nextState;
    TID         nextMember;

    // Admin
    uint64_t    index;
    ThreadState state;
};

class ThreadTable : public Structure<TID>
{
public:
	struct Config
	{
		TSize numThreads;
	};

    ThreadTable(Processor& parent, const Config& config);

    TSize getNumThreads() const { return m_threads.size(); }

          Thread& operator[](TID index)       { return m_threads[index]; }
    const Thread& operator[](TID index) const { return m_threads[index]; }

    bool empty() const { return m_numUsed == 0; }

    TID  popEmpty();
    bool pushEmpty(const ThreadQueue& queue);

    //
    // Ports
    //
    DedicatedReadPort       p_fetch;
    DedicatedWritePort<TID> p_execute;

private:
    Processor&          m_parent;
    ThreadQueue         m_empty;
    std::vector<Thread> m_threads;
    unsigned long       m_numUsed;
};

}
#endif

