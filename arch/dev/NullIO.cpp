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
#include "NullIO.h"
#include "sim/except.h"
#include <iomanip>

using namespace std;
namespace Simulator
{


    NullIO::NullIO(const std::string& name, Object& parent, Clock& clock)
        : Object(name, parent, clock)
    {
    }
    
    void NullIO::CheckEndPoints(IODeviceID from, IODeviceID to) const
    {
        if (from >= m_clients.size() || m_clients[from] == NULL)
        {
            throw exceptf<SimulationException>(*this, "I/O from non-existent device %u", (unsigned)from);
        }

        if (to >= m_clients.size() || m_clients[to] == NULL)
        {
            throw exceptf<SimulationException>(*this, "I/O from device %u to non-existence device %u", (unsigned)from, (unsigned)to);
        }

    }
  
    bool NullIO::RegisterClient(IODeviceID id, IIOBusClient& client)
    {
        if (id >= m_clients.size())
        {
            m_clients.resize(id + 1, NULL);
        }
        if (m_clients[id] != NULL)
        {
            throw exceptf<InvalidArgumentException>(*this, "Device number %zu is already registered", id);
        }
        m_clients[id] = &client;
        return true;
    }

    bool NullIO::SendReadRequest(IODeviceID from, IODeviceID to, MemAddr address, MemSize size)
    {
        CheckEndPoints(from, to);

        return m_clients[to]->OnReadRequestReceived(from, address, size);
    }

    bool NullIO::SendReadResponse(IODeviceID from, IODeviceID to, MemAddr address, const IOData& data)
    {
        CheckEndPoints(from, to);

        return m_clients[to]->OnReadResponseReceived(from, address, data);
    }

    bool NullIO::SendWriteRequest(IODeviceID from, IODeviceID to, MemAddr address, const IOData& data)
    {
        CheckEndPoints(from, to);
        return m_clients[to]->OnWriteRequestReceived(from, address, data);
    }

    bool NullIO::SendInterruptRequest(IODeviceID from, IOInterruptID which)
    {
        if (from >= m_clients.size() || m_clients[from] == NULL)
        {
            throw exceptf<SimulationException>(*this, "I/O from non-existent device %u", (unsigned)from);
        }

        bool res = true;
        for (size_t to = 0; to < m_clients.size(); ++to)
        {
            if (m_clients[to] != NULL)
                res = res & m_clients[to]->OnInterruptRequestReceived(which);
        }
        return res;
    }

    bool NullIO::SendNotification(IODeviceID from, IOInterruptID which, Integer tag)
    {
        if (from >= m_clients.size() || m_clients[from] == NULL)
        {
            throw exceptf<SimulationException>(*this, "I/O from non-existent device %u", (unsigned)from);
        }

        bool res = true;
        for (size_t to = 0; to < m_clients.size(); ++to)
        {
            if (m_clients[to] != NULL)
                res = res & m_clients[to]->OnNotificationReceived(which, tag);
        }
        return res;
    }

    IODeviceID NullIO::GetLastDeviceID() const
    {
        return m_clients.size();
    };
    void NullIO::GetDeviceIdentity(IODeviceID which, IODeviceIdentification &id) const
    {
        if (which >= m_clients.size() || m_clients[which] == NULL)
        {
            DebugIOWrite("I/O identification request to non-existent device %u", (unsigned)which);
            id.provider = 0;
            id.model = 0;
            id.revision = 0;
        }
        else
        {
            m_clients[which]->GetDeviceIdentity(id);
        }
    }

    void NullIO::Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const
    {
        out << 
            "The Null I/O bus implements a zero-latency bus between\n"
            "the components connected to it.\n\n"
            "The following components are registered:\n";
        if (m_clients.empty())
        {
            out << "(no components registered)" << endl;
        }
        else
        {
            out <<
                "  ID  |  P  / M  / R   | Name\n"
                "------+----------------+-----------\n";
        }

        for (size_t i = 0; i < m_clients.size(); ++i)
        {
            if (m_clients[i] != NULL)
            {
                IODeviceIdentification id;
                m_clients[i]->GetDeviceIdentity(id);

                out << setw(5) << setfill(' ') << i
                    << " | "
                    << setw(4) << setfill('0') << hex << id.provider
                    << '/'
                    << setw(4) << setfill('0') << hex << id.model
                    << '/'
                    << setw(4) << setfill('0') << hex << id.revision
                    << " | "
                    << m_clients[i]->GetIODeviceName()
                    << endl;
            }
        }
    }

}
