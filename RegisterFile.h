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
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "ports.h"

namespace Simulator
{

class Processor;
class ICache;
class DCache;
class Allocator;

//
// RegisterFile class
//
class RegisterFile : public Structure<RegAddr>
{
    // The register file has 2 read ports dedicated for the Read stage of the pipeline,
    // one write port from the writeback stage of the pipeline, and one asynchronous
    // read and write port for other components (network, memory, etc)
public:
	struct Config
	{
		RegSize numIntegers;
		RegSize numFloats;
	};

    RegisterFile(Processor& parent, ICache& icache, DCache &dcache, Allocator& allocator, const Config& config);

    bool readRegister(const RegAddr& addr, RegValue& data) const;
    bool writeRegister(const RegAddr& addr, const RegValue& data, const IComponent& component);
    bool clear(const RegAddr& addr, RegSize size, const RegValue& value);

	// Admin interface, do not use from simulation
    bool writeRegister(const RegAddr& addr, const RegValue& data);
    
    RegSize getSize(RegType type) const;

    DedicatedReadPort            p_pipelineR1;
    DedicatedReadPort            p_pipelineR2;
    DedicatedWritePort<RegAddr>  p_pipelineW;
    ArbitratedReadPort           p_asyncR;
    ArbitratedWritePort<RegAddr> p_asyncW;

private:
    //void onArbitrateWritePhase();

    std::vector<RegValue> m_integers;
    std::vector<RegValue> m_floats;
    Processor& m_parent;
    Allocator& m_allocator;
    ICache&    m_icache;
	DCache&    m_dcache;
};

}
#endif

