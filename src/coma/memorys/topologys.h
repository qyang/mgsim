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
#ifndef _TOPOLOGY_S_H
#define _TOPOLOGY_S_H
//////////////////////////////////////////////////////////////////////////
// single level topology

#include "predef.h"

#include "memoryst.h"
#include "busst.h"

#include "processortok.h"
#define PROCESSOR_DEF   ProcessorTOK

#include "cachel2tokim.h"
#include "directoryrttokim.h"
#define CACHEL2IM_DEF   CacheL2TOKIM
#define DIRRTIM_DEF     DirectoryRTTOKIM

#include "directorytokim.h"

#include "vmemorydatacontainer.h"
//#include "rangememorydatacontainer.h"
//#include "simplememorydatacontainer.h"

#include "network.h"

#include "ddrmemorysys.h"


#define MEMBUS_CHANNELSWITCH

#ifdef MEMBUS_CHANNELSWITCH
#include "busswitch.h"
#endif

namespace MemSim{
//{ memory simulator namespace
//////////////////////////////

#define SC_INTERLEAVE_DIRECT    0
#define SC_INTERLEAVE_KSKEW     1


class TopologyS
{
public:
    TopologyS();
    ~TopologyS();

    void PreFill();
private:
    // clocks
    sc_clock*                   m_pclk;
    sc_clock*                   m_pclkmem;

    // real data container
    VMemoryDataContainer*  m_pmemDataSim;
    //RangeMemoryDataContainer*  m_pmemDataSim;
    //SimpleMemoryDataContainer*  m_pmemDataSim;

    //////////////////////////////////////////////////////////////////////////
    // systemc modules

    // processor array
    PROCESSOR_DEF**             m_ppProcessors;

    // proc-cache bus array
    BusST**                     m_ppBus;

    // cache L2 array
    CACHEL2IM_DEF**             m_ppCacheL2;

    // network
    Network*                    m_pNet;

    // network level 0  : for multi-level configuration only
    Network**                   m_ppNetL0;

    // memoy bus
#ifdef MEMBUS_CHANNELSWITCH
    BusSwitch*                  m_pBSMem;
#else
    BusST**                     m_ppBusMem;
#endif

    // root direcotyr
    DIRRTIM_DEF**               m_ppDirectoryRoot;

    // directories
    DirectoryTOKIM**            m_ppDirectoryL0;

    // memory
    DDRMemorySys**              m_ppMem;
    //MemoryST*                   m_pMem;
};

//////////////////////////////
//} memory simulator namespace
}

#endif // define topology

