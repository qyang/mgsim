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
#include <cassert>
#include "Pipeline.h"
#include "Processor.h"
using namespace Simulator;
using namespace std;

Pipeline::PipeAction Pipeline::WritebackStage::read()
{
    return PIPE_CONTINUE;
}

Pipeline::PipeAction Pipeline::WritebackStage::write()
{
    bool suspend = m_input.suspend;
    
    if (m_input.Rc.valid() && m_input.Rcv.m_state != RST_INVALID)
    {
        if (m_input.Rrc.fid != INVALID_GFID)
        {
            assert(m_input.Rcv.m_state == RST_FULL);

            // Also forward the shared to the next CPU
            // If we're the last thread in the family, it writes to the parent thread
			if (!m_network.sendShared(m_input.Rrc.fid, m_input.isLastThreadInFamily, m_input.Rrc.reg, m_input.Rcv))
            {
                return PIPE_STALL;
            }
        }
        
        // We have something to write back  
        if (!m_regFile.p_pipelineW.write(*this, m_input.Rc))
        {
            return PIPE_STALL;
        }
        
        // If we're writing WAITING and the data is already present, Rcv's state will be set to FULL
        RegValue Rcv = m_input.Rcv;
        if (!m_regFile.writeRegister(m_input.Rc, Rcv, *this))
        {
            return PIPE_STALL;
        }

        suspend = (Rcv.m_state == RST_WAITING);
    }
    else if (suspend)
    {
        // Memory barrier, check to make sure it hasn't changed by now
        suspend = (m_threadTable[m_input.tid].dependencies.numPendingWrites != 0);
    }

    if (m_input.swch)
    {
        // We've switched threads
        if (m_input.kill)
        {
            // Kill the thread
            assert(suspend == false);
            if (!m_allocator.KillThread(m_input.tid))
            {
                return PIPE_STALL;
            }
        }
        // We're not killing, suspend or reschedule
        else if (suspend)
        {
            // Suspend the thread
            if (!m_allocator.SuspendThread(m_input.tid, m_input.pc))
            {
                return PIPE_STALL;
            }
        }
        // Reschedule thread
        else if (!m_allocator.RescheduleThread(m_input.tid, m_input.pc, *this))
        {
            // We cannot reschedule, stall pipeline
            return PIPE_STALL;
        }
    }
    
    return PIPE_CONTINUE;
}

Pipeline::WritebackStage::WritebackStage(Pipeline& parent, MemoryWritebackLatch& input, RegisterFile& regFile, Network& network, Allocator& alloc, ThreadTable& threadTable)
  : Stage(parent, "writeback", &input, NULL),
    m_input(input),
    m_regFile(regFile),
    m_network(network),
    m_allocator(alloc),
    m_threadTable(threadTable)
{
}
