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
#ifndef SMC_H
#define SMC_H

#include "arch/IOBus.h"
#include "sim/storage.h"

class Config;
namespace Simulator
{
    class Processor;
    class ActiveROM;

    class SMC : public IIOBusClient, public Object
    {
    private:
        char *m_enumdata;
        size_t m_size;


        IIOBus& m_iobus;
        IODeviceID m_devid;

        const std::vector<std::pair<RegAddr, RegValue> >& m_regs;
        const std::vector<std::pair<RegAddr, std::string> >& m_loads;
        Processor* m_cpu;

        ActiveROM* m_rom;
        IODeviceID m_romid;

        bool       m_enable_boot;   // whether to boot the linked processor
        bool       m_enable_dca;    // whether to do the initial DCA
        SingleFlag m_start_dca;
        SingleFlag m_doboot;

    public:
        SMC(const std::string& name, Object& parent, IIOBus& iobus, IODeviceID devid, 
            const std::vector<std::pair<RegAddr, RegValue> >& regs,
            const std::vector<std::pair<RegAddr, std::string> >& loads,
            Config& config);

        ~SMC();

        void Initialize();

        Process p_StartDCA;
        Process p_Boot;

        Result DoStartDCA();
        Result DoBoot();

        bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
        bool OnNotificationReceived(IONotificationChannelID which, Integer tag);
        
        StorageTraceSet GetNotificationTraces() const;

        void GetDeviceIdentity(IODeviceIdentification& id) const;
        std::string GetIODeviceName() const;
        

    };


}


#endif
