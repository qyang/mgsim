/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011  The Microgrid Project.

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
#ifndef ZLCOMA_MERGESTOREBUFFER_H
#define ZLCOMA_MERGESTOREBUFFER_H

#include "Cache.h"

namespace Simulator
{

class ZLCOMA::Cache::MergeStoreBuffer
{
    struct Entry;

    std::vector<Entry> m_entries;
    size_t m_lineSize;

public:
    MergeStoreBuffer(unsigned int size, size_t lineSize);
    ~MergeStoreBuffer();

    bool LoadBuffer(MemAddr address, MemData& data, const Line& linecache);
    bool WriteBuffer(MemAddr address, const MemData& data, const WriteAck& ack);
    bool CleanSlot(MemAddr address);
    bool IsSlotLocked(MemAddr address) const;    
    bool DumpMergedLine(MemAddr address, char* data, bool* bitmask, std::vector<WriteAck>& ack_queue);
    
private:
          Entry* GetEmptyLine();
          Entry* FindBufferItem(MemAddr address);
    const Entry* FindBufferItem(MemAddr address) const;
};

}

#endif
