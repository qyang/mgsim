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
#include "ports.h"
#include <sstream>
#include <algorithm>

namespace Simulator
{

/*static*/ std::string IllegalPortAccess::ConstructString(const Object& object, const std::string& name, const Process& src)
{
    std::stringstream ss;
    std::string from = src.GetName(), dest = object.GetFQN();
    std::transform(from.begin(), from.end(), from.begin(), toupper);
    std::transform(dest.begin(), dest.end(), dest.begin(), toupper);
    ss << "Illegal access to " << dest << "." << name << " by " << from;
    return ss.str();
}

void ArbitratedPort::AddRequest(const Process& process)
{
    // A process should not request a port more than once
    // in a cycle or Bad Things (TM) could happen
    assert(find(m_requests.begin(), m_requests.end(), &process) == m_requests.end());

    m_requests.push_back(&process);
}

void ArbitratedPort::Arbitrate()
{
    // Choose the request with the highest priority
    m_selected = NULL;

    unsigned int highest = std::numeric_limits<unsigned int>::max();
    for (ProcessList::const_iterator i = m_requests.begin(); i != m_requests.end(); ++i)
    {
        // The position in the vector is its priority
        unsigned int priority = std::find(&m_processes.front(), &m_processes.back() + 1, *i) - &m_processes.front();
        assert(priority < m_processes.size());
        if (priority < highest)
        {
            highest    = priority;
            m_selected = *i;
        }
    }
    m_requests.clear();
    
    if (m_selected != NULL)
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
IStructure::IStructure(const std::string& name, Object& parent)
    : Object(name, parent), Arbitrator(*parent.GetKernel())
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
