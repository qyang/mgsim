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
#ifndef RAUNIT_H
#define RAUNIT_H

#include "kernel.h"

namespace Simulator
{

class Processor;
class RegisterFile;

class RAUnit : public Object
{
public:
	struct Config
	{
		RegSize blockSizes[NUM_REG_TYPES];
	};

    typedef std::vector<std::pair<RegSize, LFID> > List;

    RAUnit(Processor& parent, const std::string& name, const RegisterFile& regFile, const Config& config);

    // LFID is for admin purposes only; not used for simulation
    bool Alloc(const RegSize size[NUM_REG_TYPES], LFID fid, RegIndex indices[NUM_REG_TYPES]);
    bool Free(RegIndex indices[NUM_REG_TYPES]);

    // Admin functions
    const List& GetList(RegType type) const { return m_lists[type]; }
    RegSize GetBlockSize(RegType type) const { return m_blockSizes[type]; }

private:
    List    m_lists[NUM_REG_TYPES];
    RegSize m_blockSizes[NUM_REG_TYPES];
};

};

#endif

