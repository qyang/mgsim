/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011,2012  The Microgrid Project.

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
#ifndef LCD_H
#define LCD_H

#include "arch/IOBus.h"
#include "sim/kernel.h"
#include <fstream>

class Config;

namespace Simulator
{

class LCD : public IIOBusClient, public Object
{
    IODeviceID m_devid;
    IIOBus&    m_iobus;

    char*      m_buffer;

    size_t     m_width;
    size_t     m_height;

    size_t     m_startrow;
    size_t     m_startcolumn;

    unsigned   m_bgcolor;
    unsigned   m_fgcolor;

    size_t     m_curx;
    size_t     m_cury;

    std::ofstream *m_tracefile;

    void Refresh(unsigned firstrow, unsigned lastrow) const;

public:
    LCD(const std::string& name, Object& parent, IIOBus& iobus, IODeviceID devid, Config& config);
    ~LCD();

    bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
    bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& data);

    void GetDeviceIdentity(IODeviceIdentification& id) const;

    std::string GetIODeviceName() const { return GetFQN(); }
};


}

#endif
