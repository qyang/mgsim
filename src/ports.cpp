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
#include "ports.h"
#include <sstream>
#include <algorithm>

namespace Simulator
{

/*static*/ std::string IllegalPortAccess::ConstructString(const Object& object, const std::string& name, const ArbitrationSource& src)
{
    std::stringstream ss;
    std::string from = src.first->GetFQN(), dest = object.GetFQN();
    std::transform(from.begin(), from.end(), from.begin(), toupper);
    std::transform(dest.begin(), dest.end(), dest.begin(), toupper);
    ss << "Illegal access to " << dest << "." << name << " by " << from << ", state " << src.second;
    return ss.str();
}

void ArbitratedPort::AddRequest(const ArbitrationSource& source)
{
    // A source should not request a port more than once
    // in a cycle or Bad Things (TM) could happen
    assert(find(m_requests.begin(), m_requests.end(), source) == m_requests.end());

    m_requests.push_back(source);
}

void ArbitratedPort::Arbitrate()
{
    // Choose the request with the highest priority
    m_source = ArbitrationSource();

    int highest = std::numeric_limits<int>::max();
    for (RequestList::const_iterator i = m_requests.begin(); i != m_requests.end(); ++i)
    {
        PriorityMap::const_iterator priority = m_priorities.find(*i);
        if (priority != m_priorities.end() && priority->second < highest)
        {
            highest  = priority->second;
            m_source = *i;
        }
    }
    m_requests.clear();
    
    if (m_source != ArbitrationSource())
    {
        m_busyCycles++;
    }
}

void ArbitratedService::OnArbitrate()
{
    Arbitrate();
}

//
// IStructure class
//
IStructure::IStructure(Object* parent, Kernel& kernel, const std::string& name)
    : Object(parent, &kernel, name), Arbitrator(kernel)
{
}

void IStructure::RegisterReadPort(ArbitratedReadPort& port)
{
    m_readPorts.insert(&port);
}

void IStructure::UnregisterReadPort(ArbitratedReadPort& port)
{
    m_readPorts.erase(&port);
}

void IStructure::ArbitrateReadPorts()
{
    // Arbitrate between all incoming requests
    for (ReadPortList::iterator i = m_readPorts.begin(); i != m_readPorts.end(); ++i)
    {
        (*i)->Arbitrate();
    }
}

}
