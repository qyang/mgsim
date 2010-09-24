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
#include "mergestorebuffer.h"
#include <cstring>
using namespace std;

namespace Simulator
{

struct ZLCOMA::Cache::MergeStoreBuffer::Entry
{
    bool    valid;
    MemAddr tag;
    bool    locked;
    char    data   [MAX_MEMORY_OPERATION_SIZE];
    bool    bitmask[MAX_MEMORY_OPERATION_SIZE];
    
    std::vector<WriteAck> ack_queue;
};

ZLCOMA::Cache::MergeStoreBuffer::MergeStoreBuffer(unsigned int size, size_t lineSize)
    : m_entries(size), m_lineSize(lineSize)
{
    for (size_t i = 0; i < m_entries.size(); ++i)
    {
        m_entries[i].valid = false;
    }
}

ZLCOMA::Cache::MergeStoreBuffer::~MergeStoreBuffer()
{
}

// Write to the buffer
bool ZLCOMA::Cache::MergeStoreBuffer::WriteBuffer(MemAddr address, const MemData& data, const WriteAck& ack)
{
    Entry* line = FindBufferItem(address);
    if (line == NULL)
    {
        // Allocate a new line
        line = GetEmptyLine();
        if (line == NULL)
        {
            return false;
        }

        line->valid  = true;
        line->tag    = address / m_lineSize;
        line->locked = false;
        std::fill(line->bitmask, line->bitmask + MAX_MEMORY_OPERATION_SIZE, false);
        std::fill(line->data,    line->data    + MAX_MEMORY_OPERATION_SIZE, 0);
    }
    else if (line->locked)
    {
        // Line is locked
        return false;
    }
    
    size_t offset = address % m_lineSize;

    // Merge request data into line and merged request
    std::copy(data.data, data.data + data.size, line->data + offset);
    std::fill(line->bitmask + offset, line->bitmask + offset + data.size, true);
    
    line->ack_queue.push_back(ack);
    return true;
}

// load from the buffer
bool ZLCOMA::Cache::MergeStoreBuffer::LoadBuffer(MemAddr address, MemData& data, const Line& linecache)
{
    Entry* line = FindBufferItem(address);
    if (line == NULL)
    {
        return false;
    }

    for (unsigned int i = 0; i < data.size; i++)
    {
        if (line->bitmask[i])
        {
            data.data[i] = line->data[i];
        }
        else if (linecache.bitmask[i])
        {
            data.data[i] = linecache.data[i];
        }
        else
        {
            // lock the line 
            line->locked = true;
            return false;
        }
    }
    return true;
}

bool ZLCOMA::Cache::MergeStoreBuffer::IsSlotLocked(MemAddr address) const
{
    const Entry* line = FindBufferItem(address);
    return line != NULL && line->locked;
}

ZLCOMA::Cache::MergeStoreBuffer::Entry* ZLCOMA::Cache::MergeStoreBuffer::FindBufferItem(MemAddr address)
{
    MemAddr tag = address / m_lineSize;
    for (unsigned int i = 0; i < m_entries.size(); ++i)
    {
        if (m_entries[i].valid && m_entries[i].tag == tag)
        {
            return &m_entries[i];
        }
    }
    return NULL;
}

const ZLCOMA::Cache::MergeStoreBuffer::Entry* ZLCOMA::Cache::MergeStoreBuffer::FindBufferItem(MemAddr address) const
{
    MemAddr tag = address / m_lineSize;
    for (unsigned int i = 0; i < m_entries.size(); ++i)
    {
        if (m_entries[i].valid && m_entries[i].tag == tag)
        {
            return &m_entries[i];
        }
    }
    return NULL;
}

ZLCOMA::Cache::MergeStoreBuffer::Entry* ZLCOMA::Cache::MergeStoreBuffer::GetEmptyLine()
{
    for (unsigned int i = 0 ; i < m_entries.size(); ++i)
    {
        if (!m_entries[i].valid)
        {
            return &m_entries[i];
        }
    }
    return NULL;
}

bool ZLCOMA::Cache::MergeStoreBuffer::DumpMergedLine(MemAddr address, char* data, bool* bitmask, std::vector<WriteAck>& ack_queue)
{
    Entry* line = FindBufferItem(address);
    if (line != NULL)
    {
        // Return the merged line
        std::copy(line->data,    line->data    + MAX_MEMORY_OPERATION_SIZE, data);
        std::copy(line->bitmask, line->bitmask + MAX_MEMORY_OPERATION_SIZE, bitmask);
        ack_queue.clear();
        ack_queue.swap(line->ack_queue);

        // Invalidate the line
        line->valid = false;
        return true;
    }
    return false;
}

}
