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
using namespace Simulator;
using namespace std;

void ArbitratedFunction::arbitrate()
{
    // Choose the request with the highest priority
    int highest = INT_MAX;
    m_component = NULL;
    for (RequestMap::const_iterator i = m_requests.begin(); i != m_requests.end(); i++)
    {
        PriorityMap::const_iterator priority = m_priorities.find(*i);
        if (priority != m_priorities.end() && priority->second < highest)
        {
            highest     = priority->second;
            m_component = *i;
        }
    }
    m_requests.clear();
}

#ifndef NDEBUG
void ArbitratedFunction::verify(const IComponent& component)
{
    if (m_priorities.find(&component) == m_priorities.end())
    {
        throw IllegalPortAccess(component.getFQN());
    }
}

void DedicatedFunction::verify(const IComponent& component)
{
    if (m_component != &component)
    {
        throw IllegalPortAccess(component.getFQN());
    }
}
#endif

