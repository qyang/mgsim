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
#ifndef IODEVICEDATABASE_H
#define IODEVICEDATABASE_H

#include <map>

namespace Simulator
{

struct IODeviceIdentification 
{
    uint16_t  provider;
    uint16_t  model;
    uint16_t  revision;
};

class DeviceDatabase
{
    struct ProviderEntry {
        uint16_t                    id;
        const char                  *name;
    };
    static const ProviderEntry      provider_db[];

    struct DeviceEntry {
        IODeviceIdentification      id;
        const char                  *name;
    };
    static const DeviceEntry        device_db[];


    std::map<uint16_t, std::string> m_providers;

    DeviceDatabase();

    static const DeviceDatabase     m_singleton;


public:
    static 
    const DeviceDatabase& GetDatabase() { return m_singleton; }

    void Print(std::ostream& out) const;

    bool FindDeviceByName(const std::string& provider, const std::string& model, IODeviceIdentification& id) const;
};


}


#endif
