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
#include "arch/IOBus.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace Simulator
{

    const DeviceDatabase::ProviderEntry
    DeviceDatabase::provider_db[] =
    {
        {   1, "MGSim" },

        {   0, NULL }
    };

    const DeviceDatabase::DeviceEntry
    DeviceDatabase::device_db[] =
    {
        { {   1,  0,  1 }, "CPU" },
        { {   1,  1,  1 }, "LCD" },
        { {   1,  2,  1 }, "RTC" },
        { {   1,  3,  1 }, "GfxFB" },
        { {   1,  4,  1 }, "GfxCtl" },
        { {   1,  5,  1 }, "UART" },
        { {   1,  6,  1 }, "Timer" },

        { {   0,  0,  0 }, NULL }
    };

    
    DeviceDatabase::DeviceDatabase()
    {
       for (size_t i = 0; provider_db[i].name != NULL; ++i)
           m_providers[provider_db[i].id] = provider_db[i].name;
    }
                                   
    const DeviceDatabase
    DeviceDatabase::m_singleton;

    
    void DeviceDatabase::Print(std::ostream& out) const
    {
        out << "Prov. | Mod. | Rev. | Description" << endl
            << "------+------+------+---------------" << endl;
        for (size_t i = 0; device_db[i].name != NULL; ++i)
        {
            out << setw(4) << setfill('0') << hex << device_db[i].id.provider
                << "  | "
                << setw(4) << setfill('0') << hex << device_db[i].id.model
                << " | "
                << setw(4) << setfill('0') << hex << device_db[i].id.revision
                << " | "
                << m_providers.find(device_db[i].id.provider)->second
                << " "
                << device_db[i].name
                << endl;
        }
    }
    
    bool DeviceDatabase::FindDeviceByName(const string& provider, const string& model, IODeviceIdentification& id) const
    {
        for (size_t i = 0; device_db[i].name != NULL; ++i)
        {
            if (provider == m_providers.find(device_db[i].id.provider)->second
                && model == device_db[i].name)
            {
                id = device_db[i].id;
                return true;
            }
        }
        return false;
    }

}

