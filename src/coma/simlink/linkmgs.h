/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010  The Microgrid Project.

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
#ifndef LINKMGS_H
#define LINKMGS_H
#include "../../VirtualMemory.h"
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include <iostream>

class LinkConfig
{
public:
    unsigned int m_nLineSize;
    unsigned int m_nProcs;
    unsigned int m_nProcessorsPerCache;
    unsigned int m_nCachesPerDirectory;
    unsigned int m_nNumRootDirs;
    unsigned int m_nMemoryChannels;
    unsigned int m_nCacheSet;
    unsigned int m_nCacheAssociativity;
    unsigned int m_nCacheAccessTime;
    unsigned int m_nMemoryAccessTime;
    unsigned int m_nInject;

    unsigned int m_nCycleTimeCore;
    unsigned int m_nCycleTimeMemory;

    LinkConfig()
    {
        m_nMemoryAccessTime = 100;
    }
    
    void dumpConfiguration(std::ostream& os) const
    {
#define PC(N) "# " #N << '\t' << m_ ## N << std::endl 
        os << "### begin COMA configuration" << std::endl
           << PC(nProcs)
           << PC(nProcessorsPerCache)
           << PC(nCachesPerDirectory)
           << PC(nNumRootDirs)
           << PC(nMemoryChannels)
           << PC(nCycleTimeCore)
           << PC(nCycleTimeMemory)
           << PC(nCacheAccessTime)
           << PC(nMemoryAccessTime)
           << PC(nCacheSet)
           << PC(nCacheAssociativity)
           << PC(nLineSize)
           << PC(nInject)
           << "### end COMA configuration" << std::endl;
#undef PC
    };
};

class LinkMGS
{
public:
    // link flag
    enum LINKFLAG{
        LF_NONE,	    // no request linked
        LF_AVAILABLE,	// a linked request is available
        LF_PROCESSED	// a linked request is processed
    };

    static LinkConfig s_oLinkConfig;
protected:
    // link flags
    LINKFLAG m_eRequestIni;

public:
    LinkMGS(){
        m_eRequestIni = LF_NONE;
    }

    virtual ~LinkMGS(){}

    // initiate a request to systemc
    virtual void PutRequest(uint64_t address, bool write, uint64_t size, void* data, unsigned long* ref) = 0;
    //virtual void PutRequest(unsigned int address, bool write, unsigned int size, void* data, unsigned long* ref){PutRequest((uint64_t)address, write, (unsigned int)size, data, ref);};

    // check and get a reply from the systemc
    // NULL means no reply 
    // returned address value can be found in 
    // the request structure pointed by the replied pointer as well
    // extcode: extension code for reply
    //  0 : normal read/write reply request
    //  1 : backward broadcast invalidation request
    // for IB request, address is used. the value is LINE ADDRESS, with asscociated L2 block size +++
    virtual unsigned long* GetReply(uint64_t &address, void* data, uint64_t &size, int &extcode) = 0;
    virtual unsigned long* GetReply(uint32_t &address, void* data, uint64_t &size, int &extcode){return GetReply((uint64_t&)address, data, size, extcode);}

    virtual bool RemoveReply() = 0;
};

extern Simulator::VirtualMemory* g_pMemoryDataContainer;
extern LinkMGS** g_pLinks;

#include "memstat.h"

#endif

