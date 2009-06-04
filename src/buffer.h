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
#ifndef BUFFER_H
#define BUFFER_H

#include "kernel.h"
#include <queue>

namespace Simulator
{

typedef size_t  BufferSize; // Size of buffer, in elements
static const    BufferSize INFINITE = (size_t)-1;

template <typename T>
class Buffer
{
    Kernel&         m_kernel;
    size_t          m_maxSize;
    std::queue<T>   m_data;

public:
    bool       empty() const { return m_data.empty(); }
    bool       full()  const { return (m_maxSize != INFINITE && m_data.size() == m_maxSize); }
    BufferSize size()  const { return m_data.size(); }
    const T& front() const { return m_data.front(); }
          T& front()       { return m_data.front(); }

    void pop() { if (m_kernel.GetCyclePhase() == PHASE_COMMIT) { m_data.pop(); } }
    bool push(const T& item)
    {
        if (!full())
        {
            if (m_kernel.GetCyclePhase() == PHASE_COMMIT) { m_data.push(item); }
            return true;
        }
        return false;
    }

    Buffer(Kernel& kernel, BufferSize maxSize) : m_kernel(kernel), m_maxSize(maxSize)
    {
    }
};

}
#endif

