/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011  The Microgrid Project.

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
#ifndef ANCILLARYREGISTERFILE_H
#define ANCILLARYREGISTERFILE_H


#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

/*
 * Access interface to ancillary registers. Ancillary registers
 * differ from regular (computational) registers in that
 * they are few and do not have state bits. 
 * FIXME:
 * This should really be a register file with ports and arbitration etc.
 * Accessed R/W from the memory stage, W from delegation (remote configure)
 */


/* The interface is shared between ancillary core registers and
   performance counters. */
class AncillaryRegisterInterface : public Object, public Inspect::Interface<Inspect::Read>
{
public:
    AncillaryRegisterInterface(const std::string& name, Object& parent, Clock& clock)
        : Object(name, parent, clock) {}

    virtual size_t GetNumRegisters() const = 0;
    virtual bool ReadRegister(size_t addr, Integer& data) const = 0;
    virtual bool WriteRegister(size_t addr, Integer data) = 0;

    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;

    virtual ~AncillaryRegisterInterface() {}
};

class AncillaryRegisterFile : public AncillaryRegisterInterface
{
    const size_t                  m_numRegisters;
    std::vector<Integer>          m_registers;

public:
    AncillaryRegisterFile(const std::string& name, Processor& parent, Clock& clock, Config& config);
    
    size_t GetNumRegisters() const { return m_numRegisters; }

    bool ReadRegister(size_t addr, Integer& data) const;
    bool WriteRegister(size_t addr, Integer data);
    
};


#endif
