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
#include "ThreadTable.h"
#include "Processor.h"
#include "config.h"
#include <cassert>
using namespace Simulator;
using namespace std;

ThreadTable::ThreadTable(Processor& parent, const Config& config)
  : Structure<TID>(&parent, parent.GetKernel(), "threads"),
    m_parent(parent),
    m_threads(config.getInteger<size_t>("NumThreads", 64)),
    m_numThreadsUsed(0)
{
    for (TID i = 0; i < m_threads.size(); ++i)
    {
        m_threads[i].nextMember = i + 1;
        m_threads[i].state      = TST_EMPTY;
    }
    m_threads[m_threads.size() - 1].nextMember = INVALID_TID;

    m_empty.head = 0;
    m_empty.tail = m_threads.size() - 1;
}

TID ThreadTable::PopEmpty()
{
    TID tid = m_empty.head;
    if (tid != INVALID_TID)
    {
        assert(m_threads[tid].state == TST_EMPTY);
        assert(m_numThreadsUsed < m_threads.size());
        COMMIT
        {
            m_empty.head = m_threads[tid].nextMember;
            m_threads[tid].state = TST_WAITING;
            m_numThreadsUsed++;
        }
    }
    return tid;
}

bool ThreadTable::PushEmpty(const ThreadQueue& q)
{
    assert(q.head != INVALID_TID);
    assert(q.tail != INVALID_TID);
    
    COMMIT
    {
        if (m_empty.head == INVALID_TID) {
            m_empty.head = q.head;
        } else {
            m_threads[m_empty.tail].nextMember = q.head;
        }
        m_empty.tail = q.tail;
        m_threads[q.tail].nextMember = INVALID_TID;

        // Admin, set states to empty
        for (TID cur = q.head; cur != INVALID_TID; cur = m_threads[cur].nextMember)
        {
            assert(m_numThreadsUsed > 0);
            DebugSimWrite("Decrementing for T%u", cur);
            m_threads[cur].state = TST_EMPTY;
            m_numThreadsUsed--;
        }
    }
    return true;
}
