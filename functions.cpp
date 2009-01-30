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
#include "functions.h"
#include <cassert>
using namespace Simulator;
using namespace std;

void ArbitratedFunction::arbitrate()
{
    // Choose the request with the highest priority (lowest numerical value)
    m_priority = INT_MAX;
    for (RequestMap::const_iterator i = m_requests.begin(); i != m_requests.end(); i++)
    {
        if (i->first < m_priority)
        {
            m_priority  = i->first;
            m_component = i->second;
        }
    }
    m_requests.clear();
}

void ArbitratedFunction::setPriority(const IComponent& component, int priority)
{
    m_priorities[&component] = priority;
}

void ArbitratedFunction::addRequest(const IComponent& component)
{
    assert(!m_priorities.empty());
    
    PriorityMap::const_iterator p = m_priorities.find(&component);
    assert(p != m_priorities.end());
    m_requests.insert(make_pair(p->second, &component));
}

void ArbitratedFunction::addRequest(int priority)
{
    assert(m_priorities.empty());
    
    m_requests.insert(make_pair(priority, (const IComponent*)NULL));
}

                                
#ifndef NDEBUG
void ArbitratedFunction::verify(const IComponent& component) const
{
    if (m_priorities.find(&component) == m_priorities.end())
    {
        throw IllegalPortAccess(component);
    }
}

void DedicatedFunction::verify(const IComponent& component) const
{
    if (m_component != &component)
    {
        throw IllegalPortAccess(component);
    }
}
#endif

