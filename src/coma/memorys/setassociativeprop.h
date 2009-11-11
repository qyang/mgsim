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
#ifndef _SET_ASSOCIATIVEPROP_H
#define _SET_ASSOCIATIVEPROP_H

#include "predef.h"

namespace MemSim{
//{ memory simulator namespace
//////////////////////////////


class SetAssociativeProp : virtual public SimObj
{
public:
    static unsigned int s_nLineSize;
    static unsigned int s_nLineBits;

protected:
    bool    m_bDirectory;

public:
    SetAssociativeProp(unsigned int nlinesize)
    {
        if (nlinesize <= 0)
            cerr << ERR_HEAD_OUTPUT << "illegal cacheline size" << endl;

        // update the linesize
        if (s_nLineSize == 0)
        {
            s_nLineSize = nlinesize;
            s_nLineBits = lg2(nlinesize);
            ST_request::s_nRequestAlignedSize = nlinesize;
        }
        else if (s_nLineSize != nlinesize)
        {
            cerr << ERR_HEAD_OUTPUT << "the cacheline size is inconsistent with other caches" << endl;
        }

        m_bAssoModule = true;
    }

    bool IsDirectory(){return m_bDirectory;};

    virtual void MonitorAddress(ostream& ofs, __address_t addr){};
};

//////////////////////////////
//} memory simulator namespace
}

#endif

