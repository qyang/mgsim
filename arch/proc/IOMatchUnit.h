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
#ifndef IOMATCHUNIT_H
#define IOMATCHUNIT_H

#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

class MMIOComponent;

class IOMatchUnit : public Object, public Inspect::Interface<Inspect::Info>
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
    IOMatchUnit(const std::string& name, Processor& parent, Clock& clock, const Config& config);

    Processor& GetProcessor() const;

    bool IsRegisteredReadAddress(MemAddr address, MemSize size) const;
    bool IsRegisteredWriteAddress(MemAddr address, MemSize size) const;

    Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid, const RegAddr& writeback);
    Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid);

    void RegisterComponent(MemAddr base, AccessMode mode, MMIOComponent& component);    

    // Debugging
    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
};


class MMIOComponent : public Object
{
public:
    MMIOComponent(const std::string& name, Object& parent, Clock& clock);

    virtual size_t GetSize() const = 0;


    virtual Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid, const RegAddr& writeback) = 0;
    virtual Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid) = 0;

    virtual ~MMIOComponent() {};
};




#endif
