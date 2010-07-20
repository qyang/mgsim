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
#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include "simtypes.h"
#include <map>
#include <vector>

namespace Simulator
{

// This class presents as large, sparse memory region as a linear memory region.
// It allocates blocks of memory as they are read or written.
class VirtualMemory
{
public:
    // We allocate per block, this is the size of each block. Must be a power of two
    static const int BLOCK_SIZE = (1 << 12);
    
    struct Block
    {
        char data[BLOCK_SIZE];
    };
    
    struct Range
    {
        MemSize size;
        int     permissions;
    };
    
    typedef std::map<MemAddr, Block> BlockMap;
    typedef std::map<MemAddr, Range> RangeMap;
    
    bool Allocate(MemSize size, int perm, MemAddr& address);
    
    void Reserve(MemAddr address, MemSize size, int perm);
    void Unreserve(MemAddr address);
    
    void Read (MemAddr address, void* data, MemSize size) const;
    void Write(MemAddr address, const void* data, MemSize size);
    
    bool CheckPermissions(MemAddr address, MemSize size, int access) const;

    VirtualMemory();    
    virtual ~VirtualMemory();
    
    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
private:
    RangeMap::const_iterator GetReservationRange(MemAddr address, MemSize size) const;
    

    BlockMap m_blocks;
    RangeMap m_ranges;
    size_t   m_totalreserved;
    size_t   m_totalallocated;
    size_t   m_nRanges;
};

}
#endif

