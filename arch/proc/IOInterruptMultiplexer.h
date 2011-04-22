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
#ifndef IOINTMUX_H
#define IOINTMUX_H

#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

class IOBusInterface;

class IOInterruptMultiplexer : public Object
{
private:
    RegisterFile&                   m_regFile;
    IOBusInterface&                 m_iobus;

    std::vector<Register<RegAddr>*> m_writebacks;
    std::vector<SingleFlag*>        m_interrupts;

    size_t                          m_lastNotified;

public:
    IOInterruptMultiplexer(const std::string& name, Object& parent, Clock& clock, RegisterFile& rf, IOBusInterface& iobus, const Config& config);

    // sent by device select upon an I/O read from the processor
    bool SetWriteBackAddress(IODeviceID dev, const RegAddr& addr);

    // triggered by the IOBusInterface
    bool OnInterruptRequestReceived(IODeviceID from);

    Process p_IncomingInterrupts;
    
    // upon interrupt received
    Result DoReceivedInterrupts();
};


#endif
