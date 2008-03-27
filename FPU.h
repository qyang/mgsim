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
#ifndef FPU_H
#define FPU_H

#include "kernel.h"
#include "ISA.h"
#include <queue>
#include <map>

namespace Simulator
{

class Processor;
class RegisterFile;

class FPU : public IComponent
{
	struct Result
	{
		RegAddr address;
		Float   value;
		CycleNo completion;
	};

	std::map<CycleNo, std::deque<Result> > m_pipelines;

public:
	struct Config
	{
		CycleNo addLatency;
		CycleNo subLatency;
		CycleNo mulLatency;
		CycleNo divLatency;
		CycleNo sqrtLatency;
	};

    FPU(Processor& parent, const std::string& name, RegisterFile& regFile, const Config& config);

	bool queueOperation(int opcode, int func, const Float& Rav, const Float& Rbv, const RegAddr& Rc);

    bool idle()   const;

private:
	bool onCompletion(const Result& res) const;
	Simulator::Result onCycleWritePhase(int stateIndex);

	RegisterFile& m_registerFile;
	Config        m_config;
};

}
#endif

