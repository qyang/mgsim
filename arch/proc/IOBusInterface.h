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
class IONotificationMultiplexer;
class IODirectCacheAccess;

class IOBusInterface : public IIOBusClient, public Object
{
public:
    enum IORequestType
    {
        REQ_READ,
        REQ_WRITE,
        REQ_READRESPONSE
    };

    struct IORequest
    {
        IODeviceID    device;
        IORequestType type;
        MemAddr       address;    // for all types
        IOData        data;       // for writes & read responses
    };

private:
    IOResponseMultiplexer&     m_rrmux;
    IONotificationMultiplexer& m_nmux;
    IODirectCacheAccess&       m_dca;
    IIOBus&                    m_iobus;
    IODeviceID                 m_hostid;

    Buffer<IORequest>          m_outgoing_reqs;

public:
    IOBusInterface(const std::string& name, Object& parent, Clock& clock, IOResponseMultiplexer& rrmux, IONotificationMultiplexer& nmux, IODirectCacheAccess& dca, IIOBus& iobus, IODeviceID devid, Config& config);

    bool SendRequest(const IORequest& request);
    
    Process p_OutgoingRequests;

    Result DoOutgoingRequests();


    /* From IIOBusClientCallBack */
    bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
    bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data);
    bool OnReadResponseReceived(IODeviceID from, MemAddr address, const IOData& data);
    bool OnInterruptRequestReceived(IOInterruptID which);
    bool OnNotificationReceived(IOInterruptID which, Integer tag);

    void GetDeviceIdentity(IODeviceIdentification& id) const;
    
    IODeviceID GetHostID() const { return m_hostid; }

    /* for debugging */
    std::string GetIODeviceName() const { return GetFQN(); };

};


#endif
