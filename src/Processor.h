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
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "kernel.h"
#include "Allocator.h"
#include "ICache.h"
#include "DCache.h"
#include "RegisterFile.h"
#include "Pipeline.h"
#include "Network.h"
#include "FamilyTable.h"
#include "ThreadTable.h"
#include "RAUnit.h"

class Config;

namespace Simulator
{

struct PlaceInfo
{
    PSize m_size;  ///< Number of processors in the place
};

class FPU;

class Processor : public IComponent, public IMemoryCallback
{
public:
    Processor(Object* parent, Kernel& kernel, GPID pid, LPID lpid, const std::vector<Processor*>& grid, PSize gridSize, PSize placeSize, const std::string& name, IMemory& m_memory, FPU& fpu, const Config& config, MemAddr runAddress);
    void Initialize(Processor& prev, Processor& next);

    GPID    GetPID()       const { return m_pid;       }
    PSize   GetPlaceSize() const { return m_placeSize; }
    PSize   GetGridSize()  const { return m_gridSize;  }
    Kernel& GetKernel()    const { return m_kernel;    }
    bool    IsIdle()       const;

    uint64_t GetFlop() const { return m_pipeline.GetFlop(); }
    uint64_t GetOp()   const { return m_pipeline.GetOp(); }
	float GetRegFileAsyncPortActivity() const {
		return (float)m_registerFile.p_asyncW.GetBusyCycles() / (float)m_kernel.GetCycleNo();
	}
	
	uint64_t GetTotalActiveQueueSize() const { return m_allocator.GetTotalActiveQueueSize(); }
	uint64_t GetMaxActiveQueueSize()   const { return m_allocator.GetMaxActiveQueueSize(); }
	uint64_t GetMinActiveQueueSize()   const { return m_allocator.GetMinActiveQueueSize(); }
	
	uint64_t GetMinPipelineIdleTime() const { return m_pipeline.GetMinIdleTime(); }
	uint64_t GetMaxPipelineIdleTime() const { return m_pipeline.GetMaxIdleTime(); }
	uint64_t GetAvgPipelineIdleTime() const { return m_pipeline.GetAvgIdleTime(); }
	
	float GetPipelineEfficiency() const { return m_pipeline.GetEfficiency(); }
	
	CycleNo GetLocalFamilyCompletion() const { return m_localFamilyCompletion; }
	
	void WriteRegister(const RegAddr& addr, const RegValue& value) {
		m_registerFile.WriteRegister(addr, value);
	}
	
	void OnFamilyTerminatedLocally(MemAddr pc);

	// All memory requests from caches go through the processor.
	// No memory callback specified, the processor will use the tag to determine where it came from.
	void   ReserveTLS(MemAddr address, MemSize size);
	void   UnreserveTLS(MemAddr address);
	Result ReadMemory (MemAddr address, void* data, MemSize size, MemTag tag);
	Result WriteMemory(MemAddr address, void* data, MemSize size, MemTag tag);
	bool   CheckPermissions(MemAddr address, MemSize size, int access) const;
	
	Network& GetNetwork() { return m_network; }

private:
    GPID                           m_pid;
    Kernel&                        m_kernel;
	IMemory&	                   m_memory;
	const std::vector<Processor*>& m_grid;
	PSize                          m_gridSize;
	size_t                         m_placeSize;
	FPU&                           m_fpu;
	
	// Statistics 
    CycleNo m_localFamilyCompletion; 

    // IMemoryCallback
    bool OnMemoryReadCompleted(const MemData& data);
    bool OnMemoryWriteCompleted(const MemTag& tag);
    bool OnMemorySnooped(MemAddr addr, const MemData& data);

    // The components on the chip
    Allocator       m_allocator;
    ICache          m_icache;
    DCache          m_dcache;
    RegisterFile    m_registerFile;
    Pipeline        m_pipeline;
    RAUnit          m_raunit;
    FamilyTable     m_familyTable;
    ThreadTable     m_threadTable;
    Network         m_network;
};

}
#endif

