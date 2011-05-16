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
#ifndef ZLCOMA_DIRECTORY_H
#define ZLCOMA_DIRECTORY_H

#include "Node.h"
#include "sim/inspect.h"
#include <list>

class Config;

namespace Simulator
{

class ZLCOMA::DirectoryTop : public ZLCOMA::Node
{
protected:
    friend class ZLCOMA;
    friend class ZLCOMA::Directory;
    DirectoryTop(const std::string& name, ZLCOMA& parent, Clock& clock);
};

class ZLCOMA::DirectoryBottom : public ZLCOMA::Node
{
protected:
    friend class ZLCOMA;
    friend class ZLCOMA::Directory;
    DirectoryBottom(const std::string& name, ZLCOMA& parent, Clock& clock);
};

class ZLCOMA::Directory : public ZLCOMA::Object, public Inspect::Interface<Inspect::Read>
{
public:
    struct Line
    {
        bool         valid;
        MemAddr      tag;
        unsigned int tokens;     ///< Tokens in the caches in the group
    };

protected:
    friend class ZLCOMA;
    ZLCOMA::DirectoryBottom m_bottom;
    ZLCOMA::DirectoryTop    m_top;

private:
    typedef ZLCOMA::Node::Message Message;

    ArbitratedService<CyclicArbitratedPort> p_lines;      ///< Arbitrator for access to the lines
    std::vector<Line>   m_lines;      ///< The cache lines
    size_t              m_lineSize;   ///< The size of a cache-line
    size_t              m_assoc;      ///< Number of lines in a set
    size_t              m_sets;       ///< Number of sets
    size_t              m_numTokens;  ///< Total number of tokens per cache line
    CacheID             m_firstCache; ///< ID of first cache in the ring
    CacheID             m_lastCache;  ///< ID of last cache in the ring

    // Processes
    Process p_InBottom;
    Process p_InTop;

    Line* FindLine(MemAddr address);
    Line* AllocateLine(MemAddr address);
    bool  OnMessageReceivedBottom(Message* msg);
    bool  OnMessageReceivedTop(Message* msg);
    bool  IsBelow(CacheID id) const;

    bool OnBELRead(Message *);
    bool OnBELAcquireTokens(Message *);
    bool OnBELEviction(Message *);
    bool OnBELDirNotification(Message *);

    // Processes
    Result DoInBottom();
    Result DoOutBottom();
    Result DoInTop();
    Result DoOutTop();

public:
    const Line* FindLine(MemAddr address) const;

    Directory(const std::string& name, ZLCOMA& parent, Clock& clock, CacheID firstCache, CacheID lastCache, Config& config);

    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif
