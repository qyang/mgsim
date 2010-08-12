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
#ifndef _TOPOLOGY_S_H
#define _TOPOLOGY_S_H

#include "processortok.h"
#include "cachel2tok.h"
#include "directoryrttok.h"
#include "directorytok.h"
#include "ddrmemorysys.h"
#include "../../VirtualMemory.h"

namespace MemSim
{

class TopologyS : public Simulator::VirtualMemory
{
    // clocks
    sc_clock* m_pclkroot;
    sc_clock* m_pclkmem;

    std::vector<ProcessorTOK*>   m_ppProcessors;
    std::vector<CacheL2TOK*>     m_ppCacheL2;
    std::vector<DirectoryTOK*>   m_ppDirectoryL0;
    DirectoryRTTOK*              m_pDirectoryRoot;
    DDRMemorySys*                m_pMem;

public:
    TopologyS();
    ~TopologyS();
};

}

#endif

