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
#include "Pipeline.h"
#include "Processor.h"
#include <cassert>
using namespace std;

namespace Simulator
{

Pipeline::PipeAction Pipeline::DummyStage::OnCycle()
{
    COMMIT
    {
        (CommonData&)m_output = m_input;
        m_output.suspend = m_input.suspend;
        m_output.Rrc     = m_input.Rrc;
        m_output.Rc      = m_input.Rc;
        m_output.Rcv     = m_input.Rcv;
    }
    return PIPE_CONTINUE;
}

Pipeline::DummyStage::DummyStage(const std::string& name, Pipeline& parent, Clock& clock, const MemoryWritebackLatch& input, MemoryWritebackLatch& output, const Config& /*config*/)
  : Stage(name, parent, clock),
    m_input(input),
    m_output(output)
{
}

}
