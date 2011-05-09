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
#include "IOBus.h"
#include "except.h"

namespace Simulator
{
    IIOBusClient::~IIOBusClient()
    { }

    IIOBus::~IIOBus()
    { }

    void IIOBusClient::Initialize()
    { }

    bool IIOBusClient::OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size)
    {
        throw exceptf<SimulationException>("Unsupported read request received from device %u", (unsigned)from);
    }

    bool IIOBusClient::OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data)
    {
        throw exceptf<SimulationException>("Unsupported write request received from device %u", (unsigned)from);
    }

    bool IIOBusClient::OnReadResponseReceived(IODeviceID from, MemAddr address, const IOData& data)
    {
        throw exceptf<SimulationException>("Unsupported read response received from device %u", (unsigned)from);
    }

    bool IIOBusClient::OnInterruptRequestReceived(IOInterruptID which)
    {
        return true;
    }
    bool IIOBusClient::OnNotificationReceived(IOInterruptID which, Integer tag)
    {
        return true;
    }


}
