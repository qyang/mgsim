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
#ifndef _NETWORK_H
#define _NETWORK_H

#include "predef.h"
#include "network_node.h"

namespace MemSim
{

class Network
{
    sc_clock& m_clock;
    
protected:
    std::vector<Network_Node*> m_vecLpNode;

public:
    Network(sc_clock& clock) : m_clock(clock)
    {
    }

    void ConnectNetwork()
    {
        for (size_t i = 1; i < m_vecLpNode.size(); ++i)
        {
            m_vecLpNode[i - 1]->m_fifoNetOut(m_vecLpNode[i]->m_fifoNetIn);
        }
        m_vecLpNode.back()->m_fifoNetOut(m_vecLpNode[0]->m_fifoNetIn);
    }

    void operator () (Network_if& interface_)
    {
        // bind interface with network port
        char sname[100];
        sprintf(sname, "%p_node%lu", this, (unsigned long)m_vecLpNode.size());
        m_vecLpNode.push_back(
            new Network_Node(sname, m_clock, interface_.GetNetworkFifo(), interface_.GetNetworkFifoOut())
        );
    }
};

}
#endif
