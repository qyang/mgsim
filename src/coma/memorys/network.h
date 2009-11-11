/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009  The Microgrid Project.

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
#ifndef _NETWORK_H
#define _NETWORK_H

#include "predef.h"

#include "network_port.h"

namespace MemSim{
//{ memory simulator namespace
//////////////////////////////


class Network : public sc_module, public SimObj
{

public:
    // ports
    sc_in_clk port_clk;
    Network_port port_net;

public:
    SC_HAS_PROCESS(Network);
    Network(sc_module_name nm) : sc_module(nm)
    {
        port_net.SetClockPort(&port_clk);
    }

    void ConnectNetwork(){port_net.ConnectNodes();};

    virtual void InitializeLog(const char* logName, LOG_LOCATION ll, VERBOSE_LEVEL verbose = VERBOSE_DEFAULT)
    {
        SimObj::InitializeLog(logName, ll, verbose);

        port_net.InitializeLog(logName, ll, verbose);
    }
};

//////////////////////////////
//} memory simulator namespace
}

#endif
