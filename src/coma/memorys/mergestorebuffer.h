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
#ifndef _MERGESTOREBUFFER_H
#define _MERGESTOREBUFFER_H

#include "predef.h"

namespace MemSim
{

class MergeStoreBuffer
{
    struct Entry;

    std::vector<Entry> m_entries;

public:
    MergeStoreBuffer(unsigned int size);
    ~MergeStoreBuffer();

    bool LoadBuffer(Message* req, const cache_line_t& linecache);
    bool WriteBuffer(Message* req);
    bool CleanSlot(MemAddr address);
    bool IsAddressPresent(MemAddr address) const;
    bool IsSlotLocked(MemAddr address) const;
    
    const Message&               GetMergedRequest(MemAddr address) const;
    const std::vector<Message*>& GetQueuedRequestVector(MemAddr address) const;

private:
          Entry* GetEmptyLine();
          Entry* FindBufferItem(MemAddr address);
    const Entry* FindBufferItem(MemAddr address) const;
};

}

#endif
