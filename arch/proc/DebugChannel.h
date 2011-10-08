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
#ifndef DEBUGCHANNEL_H
#define DEBUGCHANNEL_H


#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

class DebugChannel : public MMIOComponent
{
    std::ostream&  m_output;
    unsigned       m_floatprecision;

public:
    DebugChannel(const std::string& name, Object& parent, std::ostream& output);

    size_t GetSize() const;

    Result Read (MemAddr address, void* data, MemSize size, LFID fid, TID tid, const RegAddr& writeback) { return FAILED; }
    Result Write(MemAddr address, const void* data, MemSize size, LFID fid, TID tid);

};



#endif
