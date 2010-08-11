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
#include "predef.h"
#include <string>
#include "inttypes.h"
using namespace std;

#include "string.h"

#include "../simlink/linkmgs.h"
#include "directorytok.h"

namespace MemSim
{

unsigned int ST_request::s_nRequestAlignedSize;

unsigned int g_nCacheLineSize  = 0;

void ST_request::Conform2BitVecFormat()
{
    // initialize bit vector
    for (unsigned int i = 0; i < CACHE_BIT_MASK_WIDTH / 8; ++i)
        bitmask[i] = 0;

    // alignment is required 
    assert(nsize != 0);
    assert(offset % CACHE_REQUEST_ALIGNMENT == 0);

    for (unsigned int i = 0; i < g_nCacheLineSize; i += CACHE_REQUEST_ALIGNMENT)
    {
        if (i >= offset && i < offset + nsize)
        {
            unsigned int maskhigh = i / (8*CACHE_REQUEST_ALIGNMENT);
            unsigned int masklow  = i % (8*CACHE_REQUEST_ALIGNMENT);
            bitmask[maskhigh] |= (1 << masklow);
        }
    }
}

void ST_request::Conform2SizeFormat()
{
    unsigned int validsize = 0;
    unsigned int validoffset = 0;
    bool bsegstart = false;
    bool bsegend   = false;

    // 00 -> 10 -> 01 -> 01 | error
    // check whether the mask is valid to transform
    for (unsigned int i = 0; i < g_nCacheLineSize; i += CACHE_REQUEST_ALIGNMENT)
    {
        unsigned int maskhigh = i / (8*CACHE_REQUEST_ALIGNMENT);
        unsigned int masklow  = i % (8*CACHE_REQUEST_ALIGNMENT);

        if (bitmask[maskhigh] & (1 << masklow))
        {
	        assert(!bsegend);
            if (!bsegstart)
            {
                validoffset = i;
                bsegstart = true;
            }
        }
        else if (!bsegend)
        {
            if (bsegstart)
            {
                bsegend   = true;
                validsize = i - validoffset;
                bsegstart = false;
            }
        }
    }

    nsize  = validsize;
    offset = validoffset;
}

unsigned int CacheState::s_nTotalToken = 0;

bool ST_request::IsRequestWithCompleteData()
{
    if (  ((type == MemoryState::REQUEST_ACQUIRE_TOKEN_DATA)&&(tokenacquired > 0)) || ((type == MemoryState::REQUEST_DISSEMINATE_TOKEN_DATA)&&(tokenacquired > 0)) )
        return true;

    return false;
}

bool ST_request::IsRequestWithNoData()
{
    if (IsRequestWithCompleteData())
        return false;

    if ( (type == MemoryState::REQUEST_WRITE) || (type == MemoryState::REQUEST_ACQUIRE_TOKEN) || ((type == MemoryState::REQUEST_ACQUIRE_TOKEN_DATA)&&( (tokenacquired > 0)||(tokenrequested == CacheState::GetTotalTokenNum()) ) ) || (type == MemoryState::REQUEST_READ_REPLY) )
        return false;

    return true;
}

bool ST_request::IsRequestWithModifiedData()
{
    if ( (type == MemoryState::REQUEST_ACQUIRE_TOKEN) || ((type == MemoryState::REQUEST_ACQUIRE_TOKEN_DATA)&&(tokenrequested == CacheState::GetTotalTokenNum())) || ((type == MemoryState::REQUEST_WRITE_REPLY)&&(bmerged)) )
        return true;

    return false;
}

bool cache_line_t::IsLineAtCompleteState()
{
    if ( (state == CacheState::CLS_INVALID) /*|| (tokencount == 0)*/ )   // JXXX maybe not good for some policies. !!! 
        return false;

    if ((state == CacheState::CLS_SHARER)&&(pending))
    {
        for (unsigned int i=0;i<CACHE_BIT_MASK_WIDTH/8;i++)
            if (((unsigned char)bitmask[i]) != 0xff)
                return false;
    }
    else if ((state == CacheState::CLS_OWNER)&&(tokencount == 0))
    {
        return false;
    }

    return true;
}

}




