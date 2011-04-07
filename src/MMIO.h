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
#ifndef MMIO_H
#define MMIO_H

#include "simtypes.h"
#include "kernel.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>

class Config;

namespace Simulator
{

class MMIOComponent;
class Processor;

class MMIOInterface : public Object
{    
public:
    enum AccessMode
    {
        READ = 1,
        WRITE = 2,
        READWRITE = 3
    };

protected:
    struct ComponentInterface
    {
        MemSize         size;
        AccessMode      mode;
        MMIOComponent*  component;
    };

    typedef std::map<MemAddr, ComponentInterface> RangeMap;
    RangeMap m_ranges;

    RangeMap::const_iterator FindInterface(MemAddr address, MemSize size) const;

public:
    MMIOInterface(const std::string& name, Processor& parent, Clock& clock, const Config& config);

    Processor& GetProcessor() const;

    bool IsRegisteredReadAddress(MemAddr address, MemSize size) const;
    bool IsRegisteredWriteAddress(MemAddr address, MemSize size) const;

    Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid);
    Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid);

    void RegisterComponent(MemAddr base, MemSize size, AccessMode mode, MMIOComponent& component);    

    // Debuggin
    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
};


class MMIOComponent : public Object
{
public:
    MMIOComponent(const std::string& name, MMIOInterface& parent, Clock& clock);

    MMIOInterface& GetInterface() { return *static_cast<MMIOInterface*>(GetParent()); }

    virtual size_t GetSize() const = 0;


    virtual Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid) = 0;
    virtual Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid) = 0;

    virtual ~MMIOComponent() {};
};



}


#endif
