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
#include "Processor.h"
#include "sim/config.h"
#include <sstream>

namespace Simulator
{
    
    Processor::IOBusInterface::IOBusInterface(const std::string& name, Object& parent, Clock& clock, IOResponseMultiplexer& rrmux, IONotificationMultiplexer& nmux, IODirectCacheAccess& dca, IIOBus& iobus, IODeviceID devid, Config& config)
        : Object(name, parent, clock),
          m_rrmux(rrmux),
          m_nmux(nmux),
          m_dca(dca),
          m_iobus(iobus),
          m_hostid(devid),
          m_outgoing_reqs("b_outgoing_reqs", *this, clock, config.getValue<BufferSize>(*this, "OutgoingRequestQueueSize")),
          p_OutgoingRequests("outgoing-requests", delegate::create<IOBusInterface, &Processor::IOBusInterface::DoOutgoingRequests>(*this))
    {
        if (m_outgoing_reqs.GetMaxSize() < 3)
        {
            throw InvalidArgumentException(*this, "OutgoingRequestQueueSize must be at least 3 to accomodate pipeline hazards");
        }
        iobus.RegisterClient(devid, *this);
        m_outgoing_reqs.Sensitive(p_OutgoingRequests);
    }

    bool Processor::IOBusInterface::SendRequest(const IORequest& request)
    {
        return m_outgoing_reqs.Push(request);
    }

    Result Processor::IOBusInterface::DoOutgoingRequests()
    {
        assert(!m_outgoing_reqs.Empty());

        const IORequest& req = m_outgoing_reqs.Front();

        switch(req.type)
        {
        case REQ_WRITE:
            if (!m_iobus.SendWriteRequest(m_hostid, req.device, req.address, req.data))
            {
                DeadlockWrite("Unable to send I/O write request to %u:%016llx (%u)",
                              (unsigned)req.device, (unsigned long long)req.address, (unsigned)req.data.size);
                return FAILED;
            }
            break;
        case REQ_READ:
            if (!m_iobus.SendReadRequest(m_hostid, req.device, req.address, req.data.size))
            {
                DeadlockWrite("Unable to send I/O read request to %u:%016llx (%u)",
                              (unsigned)req.device, (unsigned long long)req.address, (unsigned)req.data.size);
                return FAILED;
            }
            break;
        case REQ_READRESPONSE:
            if (!m_iobus.SendReadResponse(m_hostid, req.device, req.address, req.data))
            {
                DeadlockWrite("Unable to send DCA read response to %u:%016llx (%u)",
                              (unsigned)req.device, (unsigned long long)req.address, (unsigned)req.data.size);
                return FAILED;
            }
            break;
        };
        
        m_outgoing_reqs.Pop();
        return SUCCESS;
    }

    bool Processor::IOBusInterface::OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size)
    {
        IODirectCacheAccess::Request req;
        req.client = from;
        req.address = address;
        req.type = (address == 0 && size == 0) ? IODirectCacheAccess::FLUSH : IODirectCacheAccess::READ;
        req.data.size = size;
        return m_dca.QueueRequest(req);
    }

    bool Processor::IOBusInterface::OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data)
    {
        if (data.size > MAX_MEMORY_OPERATION_SIZE)
        {
            throw exceptf<InvalidArgumentException>(*this, "Write request for %#016llx/%u from client %u is too large", 
                                                    (unsigned long long)address, (unsigned)data.size, (unsigned)from);
        }

        IODirectCacheAccess::Request req;
        req.client = from;
        req.address = address;
        req.type = IODirectCacheAccess::WRITE;
        memcpy(req.data.data, data.data, data.size);
        req.data.size = data.size;
        return m_dca.QueueRequest(req);
    }

    bool Processor::IOBusInterface::OnReadResponseReceived(IODeviceID from, MemAddr address, const IOData& data)
    {
        return m_rrmux.OnReadResponseReceived(from, address, data);
    }

    bool Processor::IOBusInterface::OnInterruptRequestReceived(IOInterruptID which)
    {
        return m_nmux.OnInterruptRequestReceived(which);
    }

    bool Processor::IOBusInterface::OnNotificationReceived(IOInterruptID which, Integer tag)
    {
        return m_nmux.OnNotificationReceived(which, tag);
    }

    void Processor::IOBusInterface::GetDeviceIdentity(IODeviceIdentification& id) const
    {
        if (!DeviceDatabase::GetDatabase().FindDeviceByName("MGSim", "CPU", id))
        {
            throw InvalidArgumentException(*this, "Device identity not registered");
        }    
    }

}
