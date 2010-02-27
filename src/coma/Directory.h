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
#ifndef COMA_DIRECTORY_H
#define COMA_DIRECTORY_H

#include "Node.h"
#include <queue>
#include <set>

class Config;

namespace Simulator
{

class COMA::DirectoryTop : public COMA::Node
{
protected:
    DirectoryTop(const std::string& name, COMA& parent);
};

class COMA::DirectoryBottom : public COMA::Node
{
protected:
    DirectoryBottom(const std::string& name, COMA& parent);
};

class COMA::Directory : public COMA::DirectoryBottom, public COMA::DirectoryTop
{
public:
    struct Line
    {
        bool    valid;  ///< Valid line?
        MemAddr tag;    ///< Tag of this line
    };
    
private:
    ArbitratedService p_lines;      ///< Arbitrator for access to the lines
    std::vector<Line> m_lines;      ///< The cache lines
    size_t            m_lineSize;   ///< The size of a cache-line
    size_t            m_assoc;      ///< Number of lines in a set
    size_t            m_sets;       ///< Number of sets
    bool              m_topLevel;   ///< Is this a directory in the top-level ring?
    size_t            m_numCaches;  ///< The number of caches that lie under the directory
    
    // Processes
    Process p_InPrevBottom;
    Process p_InNextBottom;
    Process p_OutNextBottom;
    Process p_OutPrevBottom;
    Process p_InPrevTop;
    Process p_InNextTop;
    Process p_OutNextTop;
    Process p_OutPrevTop;    

    Line* FindLine(MemAddr address);
    Line* AllocateLine(MemAddr address);
    bool  OnRequestReceivedBottom(Message* msg);
    bool  OnRequestReceivedTop(Message* msg);
    bool  OnResponseReceivedTop(Message* msg);
    bool  OnResponseReceivedBottom(Message* msg);
    
    // Processes
    Result DoInPrevBottom();
    Result DoInNextBottom();
    Result DoOutNextBottom();
    Result DoOutPrevBottom();
    Result DoInPrevTop();
    Result DoInNextTop();
    Result DoOutNextTop();
    Result DoOutPrevTop();    

public:
    const Line* FindLine(MemAddr address) const;

    // numCaches is the number of caches below this directory
    Directory(const std::string& name, COMA& parent, bool top_level, size_t numCaches, const Config& config);
    
    void Cmd_Help(std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const;
};

}
#endif
