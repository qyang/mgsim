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
#ifndef _CACHEST_H
#define _CACHEST_H

#include "predef.h"

namespace MemSim
{

class CacheST : public sc_module, public CacheState, virtual public SimObj
{
protected:
    unsigned int   m_nLineSize;
    unsigned int   m_nSet;            // number of set
    unsigned int   m_nAssociativity;  // the associativity
    cache_set_t   *m_pSet;
    char *         m_pBufData;              // data buffer

public:
    CacheST(sc_module_name nm, unsigned int nset, unsigned int nassoc, unsigned int nlinesize);   
    ~CacheST();

    // inline cache set index function
    virtual unsigned int CacheIndex(__address_t address)
    {
        return (address / m_nLineSize) % m_nSet;
    }

    // inline cache line tag function
    virtual uint64 CacheTag(__address_t address)
    {
        return (address / m_nLineSize) / m_nSet;
    }

    virtual __address_t AlignAddress4Cacheline(__address_t address)
    {
        // align address to the starting of the cacheline
        return (address / m_nLineSize) * m_nLineSize;
    }
};

}
#endif
