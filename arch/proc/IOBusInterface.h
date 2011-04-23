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
#ifndef IOBUSINTERFACE_H
#define IOBUSINTERFACE_H

#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

class IOResponseMultiplexer;
class IOInterruptMultiplexer;

class IOBusInterface : public IIOBusClient, public Object
{
public:
    struct IORequest
    {
        IODeviceID  device;
        MemAddr     address;    // for reads&writes
        bool        write;
        IOData      data;       // for writes
    };

private:
    IOResponseMultiplexer&  m_rrmux;
    IOInterruptMultiplexer& m_intmux;
    IIOBus&                 m_iobus;
    IODeviceID              m_hostid;

    Buffer<IORequest>       m_outgoing_reqs;
    Buffer<IODeviceID>      m_outgoing_acks;

public:
    IOBusInterface(const std::string& name, Object& parent, Clock& clock, IOResponseMultiplexer& rrmux, IOInterruptMultiplexer& intmux, IIOBus& iobus, IODeviceID devid, const Config& config);

    bool SendInterruptAck(IODeviceID to);

    bool SendRequest(const IORequest& request);
    
    Process p_OutgoingRequests;
    Process p_OutgoingInterruptAcks;

    Result DoOutgoingRequests();
    Result DoOutgoingInterruptAcks();


    /* From IIOBusClientCallBack */
    bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
    bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data);
    bool OnReadResponseReceived(IODeviceID from, const IOData& data);
    bool OnInterruptRequestReceived(IODeviceID from);
    bool OnInterruptAckReceived(IODeviceID from);

    /* for debugging */
    std::string GetIODeviceName() const { return GetFQN(); };

};


#endif
