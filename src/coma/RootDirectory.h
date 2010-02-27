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
#ifndef COMA_ROOTDIRECTORY_H
#define COMA_ROOTDIRECTORY_H

#include "Directory.h"
#include "DDR.h"
#include <queue>
#include <set>

class Config;

namespace Simulator
{

class DDRChannel;

class COMA::RootDirectory : public COMA::DirectoryBottom, public DDRChannel::ICallback
{
public:    
    enum LineState
    {
        LINE_EMPTY,
        LINE_LOADING,
        LINE_FULL
    };
    
    struct Line
    {
        LineState    state;    ///< State of the line
        MemAddr      tag;      ///< Tag of this line
        unsigned int hops;     ///< Loading: hops to the cache that wants the reply
    };

private:
    struct MemRequest
    {
        MemAddr address;
        MemData data;
    };
    
    ArbitratedService p_lines;      ///< Arbitrator for access to the lines
    std::vector<Line> m_lines;      ///< The cache lines
    size_t            m_lineSize;   ///< The size of a cache-line
    size_t            m_assoc;      ///< Number of lines in a set
    size_t            m_sets;       ///< Number of sets
    size_t            m_numCaches;
    DDRChannel*       m_memory;     ///< DDR memory channel

    Buffer<MemRequest> m_incoming;    ///< To memory
    Buffer<Message*>   m_outgoing;    ///< From memory
    
    // Processes
    Process p_InPrevBottom;
    Process p_InNextBottom;
    Process p_OutNextBottom;
    Process p_OutPrevBottom;
    Process p_Incoming;
    Process p_Outgoing;
    
    Line* FindLine(MemAddr address, bool check_only);
    bool  OnRequestReceived(Message* msg);
    bool  OnResponseReceived(Message* msg);
    bool  OnReadCompleted(MemAddr address, const MemData& data);
    
    // Processes
    Result DoInPrevBottom();
    Result DoInNextBottom();
    Result DoOutNextBottom();
    Result DoOutPrevBottom();
    Result DoIncoming();
    Result DoOutgoing();

public:
    RootDirectory(const std::string& name, COMA& parent, VirtualMemory& memory, size_t numCaches, const Config& config);
    ~RootDirectory();
    
    // Administrative
    const Line* FindLine(MemAddr address) const;

    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif
