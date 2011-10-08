/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011,2012  The Microgrid Project.

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
#include "Processor.h"
#include "sim/config.h"
#include <iomanip>

using namespace std;

namespace Simulator
{
    void Processor::AncillaryRegisterFile::Cmd_Info(std::ostream& out, const std::vector<std::string>& /*arguments*/) const
    {
        out <<
            "The ancillary registers hold information common to all threads on a processor.\n"
            "They allow for faster (1-cycle) access to commonly used information.\n";
    }

    void Processor::AncillaryRegisterFile::Cmd_Read(std::ostream& out, const std::vector<std::string>& /*arguments*/) const
    {
        out << " Register | Value" << endl
            << "----------+----------------------" << endl;

        size_t numRegisters = GetNumRegisters();
        for (size_t i = 0; i < numRegisters; ++i)
        {
            Integer value = ReadRegister(i);
            out << setw(9) << setfill(' ') << right << i << left
                << " | "
                << setw(16 - sizeof(Integer) * 2) << setfill(' ') << ""
                << setw(     sizeof(Integer) * 2) << setfill('0') << hex << right << value << left
                << endl;
        }
    }


    Integer Processor::AncillaryRegisterFile::ReadRegister(ARAddr addr) const
    {
        if (addr >= m_numRegisters)
        {
            throw exceptf<InvalidArgumentException>(*this, "Invalid ancillary register number: %u", (unsigned)addr);
        }
        return m_registers[addr];
    }
    
    void Processor::AncillaryRegisterFile::WriteRegister(ARAddr addr, Integer data)
    {
        if (addr >= m_numRegisters)
        {
            throw exceptf<InvalidArgumentException>(*this, "Invalid ancillary register number: %u", (unsigned)addr);
        }
        m_registers[addr] = data;
    }


    Processor::AncillaryRegisterFile::AncillaryRegisterFile(const std::string& name, Processor& parent, Clock& clock, Config& config)
        : Object(name, parent, clock),
          m_numRegisters(name == "aprs" ? config.getValue<size_t>(*this, "NumAncillaryRegisters") : NUM_ASRS)
    {
        if (m_numRegisters == 0)
        {
            throw InvalidArgumentException(*this, "NumAncillaryRegisters must be 1 or larger");
        }

        m_registers.resize(m_numRegisters, 0);

        if (name == "asrs")
        {
            WriteRegister(ASR_SYSTEM_VERSION, ASR_SYSTEM_VERSION_VALUE);
        }
    }


}
