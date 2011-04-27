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
#ifndef NULLIO_H
#define NULLIO_H

#include "arch/IOBus.h"
#include "sim/kernel.h"
#include "sim/inspect.h"

#include <vector>

namespace Simulator
{
    /*
     * NullIO: an I/O bus with no latency between send and
     * receive. 
     * - Devices are numbered from 0 to N with no holes.
     * - requests to invalid devices fault the simulation.
     */
    class NullIO : public IIOBus, public Object, public Inspect::Interface<Inspect::Info>
    {
        std::vector<IIOBusClient*> m_clients;

        void CheckEndPoints(IODeviceID from, IODeviceID to) const;

    public:
        NullIO(const std::string& name, Object& parent, Clock& clock);

        /* from IIOBus */ 
        bool RegisterClient(IODeviceID id, IIOBusClient& client);

        bool SendReadRequest(IODeviceID from, IODeviceID to, MemAddr address, MemSize size);
        bool SendReadResponse(IODeviceID from, IODeviceID to, const IOData& data);
        bool SendWriteRequest(IODeviceID from, IODeviceID to, MemAddr address, const IOData& data);
        bool SendInterruptRequest(IODeviceID from, IOInterruptID which);

        void GetDeviceIdentity(IODeviceID which, IODeviceIdentification& id) const;
       

        /* debug */
        void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
        
    };
}

#endif
