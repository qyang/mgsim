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
#ifndef IOBUS_H
#define IOBUS_H

#include "simtypes.h"
#include "arch/dev/IODeviceDatabase.h"

namespace Simulator
{

typedef size_t  IODeviceID;     ///< Number of a device on an I/O Bus
typedef size_t  IOInterruptID;  ///< Number of an interrupt channel on an I/O bus

/* maximum size of the data in an I/O request. */
static const size_t MAX_IO_OPERATION_SIZE = 64;

/* the data for an I/O request. */
struct IOData
{
    char    data[MAX_IO_OPERATION_SIZE];
    MemSize size;
};

class IIOBusClient
{
public:
    virtual bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size) = 0;
    virtual bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data) = 0;
    virtual bool OnReadResponseReceived(IODeviceID from, const IOData& data) = 0;
    virtual bool OnInterruptRequestReceived(IOInterruptID which) = 0;
    virtual bool OnNotificationReceived(IOInterruptID which, Integer tag) = 0;

    virtual void GetDeviceIdentity(IODeviceIdentification& id) const = 0;

    /* for debugging */
    virtual std::string GetIODeviceName() const = 0;

    virtual ~IIOBusClient() {}
};

class Clock;

class IIOBus
{
public:
    virtual bool RegisterClient(IODeviceID id, IIOBusClient& client) = 0;

    virtual bool SendReadRequest(IODeviceID from, IODeviceID to, MemAddr address, MemSize size) = 0;
    virtual bool SendWriteRequest(IODeviceID from, IODeviceID to, MemAddr address, const IOData& data) = 0;
    virtual bool SendReadResponse(IODeviceID from, IODeviceID to, const IOData& data) = 0;
    virtual bool SendInterruptRequest(IODeviceID from, IOInterruptID which) = 0;
    virtual bool SendNotification(IODeviceID from, IOInterruptID which, Integer tag) = 0;

    virtual void GetDeviceIdentity(IODeviceID which, IODeviceIdentification& id) const = 0;

    virtual Clock& GetClock() = 0;

    virtual ~IIOBus() {}
};

}

#endif
