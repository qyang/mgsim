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
#ifndef RTC_H
#define RTC_H

#include "arch/IOBus.h"
#include "sim/kernel.h"
#include "sim/config.h"
#include "sim/storage.h"

#include <ctime>
#include <sys/time.h>

namespace Simulator
{

    typedef unsigned long long precise_time_t;
    typedef unsigned long      clock_delay_t;
       
    class RTC : public Object
    {
        bool            m_timerTicked;

        precise_time_t  m_timeOfLastInterrupt;
        clock_delay_t   m_triggerDelay;
        bool            m_deliverAllEvents;

        SingleFlag      m_enableCheck;


        class RTCInterface : public IIOBusClient, public Object
        {
            IODeviceID      m_devid;
            IIOBus&         m_iobus;
            SingleFlag      m_doNotify;
            IOInterruptID   m_interruptNumber;

            RTC&            GetRTC() { return *dynamic_cast<RTC*>(GetParent()); }

        public:
            RTCInterface(const std::string& name, RTC& parent, IIOBus& iobus, IODeviceID devid);

            Process p_notifyTime;

            Result DoNotifyTime();

            friend class RTC;

            bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
            bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data);
            void GetDeviceIdentity(IODeviceIdentification& id) const;
            
            std::string GetIODeviceName() const { return GetFQN(); }
        };

        RTCInterface         m_businterface;
        friend class RTCInterface;
        
    public:

        RTC(const std::string& name, Object& parent, Clock& clock, IIOBus& iobus, IODeviceID devid, Config& config);

        Process p_checkTime;

        Result DoCheckTime();

    };


}

#endif
