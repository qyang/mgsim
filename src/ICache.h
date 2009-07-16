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
#ifndef ICACHE_H
#define ICACHE_H

#include "storage.h"
#include "Memory.h"

class Config;

namespace Simulator
{

class Processor;
class Allocator;

class ICache : public IComponent
{
	/// A Cache-line
    struct Line
    {
		bool		  used;			///< Line used or empty?
        MemAddr       tag;			///< Address tag
        char*         data;			///< The line data
        CycleNo       access;		///< Last access time (for LRU replacement)
		bool          creation;		///< Is the family creation process waiting on this line?
        ThreadQueue	  waiting;		///< Threads waiting on this line
		unsigned long references;	///< Number of references to this line
		bool          fetched;		///< To verify that we're actually reading a fetched line
	};
	
	/// Outgoing requests
	struct Request
    {
        MemAddr address;
        MemTag  tag;
    };

    Result Fetch(MemAddr address, MemSize size, TID* tid, CID* cid);
    Result FindLine(MemAddr address, Line* &line);
    
    // Component
    Result OnCycle(unsigned int stateIndex);

    Processor&        m_parent;
	Allocator&        m_allocator;
    std::vector<Line> m_lines;
	std::vector<char> m_data;
	Buffer<Request>   m_outgoing;
	Buffer<CID>       m_incoming;
    uint64_t          m_numHits;
    uint64_t          m_numMisses;
    size_t            m_lineSize;
    size_t            m_assoc;

public:
    ICache(Processor& parent, const std::string& name, Allocator& allocator, const Config& config);
    ~ICache();
    
    ArbitratedService p_service;
    
    Result Fetch(MemAddr address, MemSize size, CID& cid);				// Initial family line fetch
    Result Fetch(MemAddr address, MemSize size, TID& tid, CID& cid);	// Thread code fetch
    bool   Read(CID cid, MemAddr address, void* data, MemSize size) const;
    bool   ReleaseCacheLine(CID bid);
    bool   IsEmpty() const;
    bool   OnMemoryReadCompleted(const MemData& data);
    size_t GetLineSize() const { return m_lineSize; }
    
    // Debugging
    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif

