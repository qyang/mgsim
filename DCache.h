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
#ifndef DCACHE_H
#define DCACHE_H

#include <queue>
#include "kernel.h"
#include "functions.h"
#include "Memory.h"

namespace Simulator
{

class Processor;
class Allocator;
class FamilyTable;
class RegisterFile;

class DCache : public IComponent
{
public:
	struct Config
	{
		size_t		assoc;
		size_t		sets;
		size_t		lineSize;
	};

    struct LineQueue
    {
        CID head;
        CID tail;
    };

    enum LineState
    {
        LINE_UNUSED,
        LINE_LOADING,
        LINE_PROCESSING,
        LINE_USED
    };

    struct Line
    {
        LineState   state;
        MemAddr     tag;
        char*       data;
        CycleNo     access;
        CID         next;
		RegAddr		waiting;
    };

    DCache(Processor& parent, const std::string& name, Allocator& allocator, FamilyTable& familyTable, RegisterFile& regFile, const Config& config);
    ~DCache();

    Result read (MemAddr address, void* data, MemSize size, LFID fid, RegAddr* reg);
    Result write(MemAddr address, void* data, MemSize size, LFID fid);

    // IMemoryCallback
    bool onMemoryReadCompleted(const MemData& data);
    bool onMemoryWriteCompleted(const MemTag& tag);
    bool onMemorySnooped(MemAddr addr, const MemData& data);
    bool idle()   const { return m_numWaiting == 0; }

    // Component
    Result onCycleWritePhase(int stateIndex);

    // Ports
    ArbitratedWriteFunction p_request;

    // Admin information
    size_t getAssociativity() const { return m_assoc; }
    size_t getLineSize()      const { return m_lineSize; }
    size_t getNumLines()      const { return m_lines.size(); }
    size_t getNumSets()       const { return m_lines.size() / m_assoc; }
    size_t getNumPending()    const { return m_numWaiting; }

    const Line& getLine(size_t i) const { return m_lines[i];  }
    uint64_t    getNumHits()      const { return m_numHits;   }
    uint64_t    getNumMisses()    const { return m_numMisses; }

private:
    Result findLine(MemAddr address, Line* &line, bool reset = true);

    Processor&           m_parent;
    Allocator&			 m_allocator;
	FamilyTable&		 m_familyTable;
	RegisterFile&		 m_regFile;

    std::vector<Line>    m_lines;
    size_t               m_lineSize;
    size_t               m_assoc;
    size_t               m_assocLog;
    unsigned long        m_numWaiting;
    LineQueue            m_returned;
    uint64_t             m_numHits;
    uint64_t             m_numMisses;
};

}
#endif

