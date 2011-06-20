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
#ifndef IOINTERFACE_H
#define IOINTERFACE_H

#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

#include "IOResponseMultiplexer.h"
#include "IONotificationMultiplexer.h"
#include "IOBusInterface.h"
#include "IODirectCacheAccess.h"

class IOInterface : public Object, public Inspect::Interface<Inspect::Info>
{
public:
    class AsyncIOInterface : public MMIOComponent, public Inspect::Interface<Inspect::Info>
    {
    private:
        MemAddr                 m_baseAddr;
        unsigned                m_devAddrBits;

        IOInterface&  GetInterface() const;
    public:
        AsyncIOInterface(const std::string& name, IOInterface& parent, Clock& clock, Config& config);

        size_t GetSize() const;

        Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid, const RegAddr& writeback);
        Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid);

        void Cmd_Info(std::ostream& out, const std::vector<std::string>& args) const;

        // for boot sequence
        unsigned GetDeviceAddressBits() const { return m_devAddrBits; }
        MemAddr GetDeviceBaseAddress(IODeviceID dev) const;
    };

    class PNCInterface : public MMIOComponent, public Inspect::Interface<Inspect::Info>
    {
    private:
        MemAddr                 m_baseAddr;
        IOInterface&  GetInterface() const;

    public:
        PNCInterface(const std::string& name, IOInterface& parent, Clock& clock, Config& config);

        size_t GetSize() const;

        Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid, const RegAddr& writeback);
        Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid);

        void Cmd_Info(std::ostream& out, const std::vector<std::string>& args) const;

        // for boot sequence
        MemAddr GetDeviceBaseAddress(IODeviceID dev) const;
    };
    

private:
    size_t                      m_numDevices;
    size_t                      m_numChannels;

    friend class AsyncIOInterface;
    AsyncIOInterface            m_async_io;

    friend class PNCInterface;
    PNCInterface                m_pnc;        

    IOResponseMultiplexer       m_rrmux;
    IONotificationMultiplexer   m_nmux;
    IOBusInterface              m_iobus_if;
    IODirectCacheAccess         m_dca;

    bool Read(IODeviceID dev, MemAddr address, MemSize size, const RegAddr& writeback);
    bool Write(IODeviceID dev, MemAddr address, const IOData& data);
    bool WaitForNotification(IONotificationChannelID dev, const RegAddr& writeback);
    bool ConfigureNotificationChannel(IONotificationChannelID dev, Integer mode);
    Processor& GetProcessor() const;

public:
    IOInterface(const std::string& name, Processor& parent, Clock& clock, IMemory& memory, RegisterFile& rf, Allocator& alloc, IIOBus& iobus, IODeviceID devid, Config& config);

    MMIOComponent& GetAsyncIOInterface() { return m_async_io; }
    MMIOComponent& GetPNCInterface() { return m_pnc; }

    IOResponseMultiplexer& GetReadResponseMultiplexer() { return m_rrmux; }
    IONotificationMultiplexer& GetNotificationMultiplexer() { return m_nmux; }
    IODirectCacheAccess& GetDirectCacheAccess() { return m_dca; }
    
    MemAddr GetDeviceBaseAddress(IODeviceID dev) const { return m_async_io.GetDeviceBaseAddress(dev); }

    // At core initialization, triggered by the SMC
    void Initialize(IODeviceID smcid);

    // Debugging
    void Cmd_Info(std::ostream& out, const std::vector<std::string>& args) const;
};


#endif
