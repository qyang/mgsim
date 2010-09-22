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
#ifndef EVICTED_DIR_LINE_BUFFER_H
#define EVICTED_DIR_LINE_BUFFER_H

#include "predef.h"
#include <map>
#include <utility>

namespace MemSim
{

class EvictedDirLineBuffer
{
    struct EDL_Content
    {
        unsigned int nrequestin;
        unsigned int ntokenrem;
    };

    std::map<MemAddr, EDL_Content> m_mapEDL;

public:
    void AddEvictedLine(MemAddr lineaddr, unsigned int requestin, unsigned int tokenrem)
    {
        EDL_Content ec = {requestin, tokenrem};
        m_mapEDL.insert(std::pair<MemAddr, EDL_Content>(lineaddr, ec));
    }

    bool FindEvictedLine(MemAddr lineaddr, unsigned int& requestin, unsigned int& tokenrem) const
    {
        std::map<MemAddr, EDL_Content>::const_iterator iter = m_mapEDL.find(lineaddr);
        if (iter != m_mapEDL.end())
        {
            requestin = iter->second.nrequestin;
            tokenrem  = iter->second.ntokenrem;
            return true;
        }
        return false;
    }

    bool FindEvictedLine(MemAddr lineaddr) const
    {
        return m_mapEDL.find(lineaddr) != m_mapEDL.end();
    }

    // incoming : true  - incoming request
    //            false - outgoing request
    bool UpdateEvictedLine(MemAddr lineaddr, bool incoming, unsigned int reqtoken, bool eviction=false)
    {
        std::map<MemAddr, EDL_Content>::iterator iter = m_mapEDL.find(lineaddr);
        if (iter != m_mapEDL.end())
        {
            if (incoming)
            {
                if (!eviction)
                    iter->second.nrequestin++;
                iter->second.ntokenrem += reqtoken;
            }
            else
            {
                if (!eviction)
                    iter->second.nrequestin--;
                iter->second.ntokenrem -= reqtoken;
            }
    
            if (iter->second.nrequestin == 0 && iter->second.ntokenrem == 0)
            {
                m_mapEDL.erase(iter);
            }
            return true;
        }
        return false;
    }

    bool DumpEvictedLine2Line(MemAddr lineaddr, unsigned int& requestin, unsigned int& tokenrem)
    {
        std::map<MemAddr, EDL_Content>::iterator iter = m_mapEDL.find(lineaddr);
        if (iter != m_mapEDL.end())
        {
            requestin = iter->second.nrequestin;
            tokenrem  = iter->second.ntokenrem;

            m_mapEDL.erase(iter);
            return true;
        }
        return false;
    }
};

}

#endif
