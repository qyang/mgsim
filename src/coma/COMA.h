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
#ifndef COMA_COMA_H
#define COMA_COMA_H

#include "../Memory.h"
#include "../storage.h"
#include "../VirtualMemory.h"
#include <queue>
#include <set>

class Config;

namespace Simulator
{

class COMA : public Object, public IMemoryAdmin, public VirtualMemory
{
public:
    class Node;
    class DirectoryTop;
    class DirectoryBottom;
    class Directory;
    class RootDirectory;
    class Cache;
    
    // A simple base class for all COMA objects. It keeps track of what
    // COMA memory it's in.
    class Object : public virtual Simulator::Object
    {
    protected:
        COMA& m_parent;
        
    public:
        Object(const std::string& name, COMA& parent)
            : Simulator::Object(name, parent), m_parent(parent) {}
        virtual ~Object() {}
    };

private:    
    typedef std::set<MemAddr> TraceMap;
    typedef size_t            CacheID;
    
    size_t                      m_numProcsPerCache;
    size_t                      m_numCachesPerDir;
    std::vector<Cache*>         m_caches;             ///< List of caches
    std::vector<Directory*>     m_directories;        ///< List of directories
    std::vector<RootDirectory*> m_roots;              ///< List of root directories
    TraceMap                    m_traces;             ///< Active traces
    
    uint64_t                    m_nreads, m_nwrites, m_nread_bytes, m_nwrite_bytes;
    
public:
    COMA(const std::string& name, Simulator::Object& parent, Clock& clock, const Config& config);
    ~COMA();

    const TraceMap& GetTraces() const { return m_traces; }
    
    // IMemory
    void Reserve(MemAddr address, MemSize size, int perm);
    void Unreserve(MemAddr address);
    void RegisterClient  (PSize pid, IMemoryCallback& callback, const Process* processes[]);
    void UnregisterClient(PSize pid);
    bool Read (PSize pid, MemAddr address, MemSize size);
    bool Write(PSize pid, MemAddr address, const void* data, MemSize size, TID tid);
    bool CheckPermissions(MemAddr address, MemSize size, int access) const;

    void GetMemoryStatistics(uint64_t& nreads, uint64_t& nwrites, uint64_t& nread_bytes, uint64_t& nwrite_bytes) const;

    void Cmd_Help (std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Line (std::ostream& out, const std::vector<std::string>& arguments) const;
    void Cmd_Trace(std::ostream& out, const std::vector<std::string>& arguments);

    // IMemoryAdmin
    bool Allocate(MemSize size, int perm, MemAddr& address);
    void Read (MemAddr address, void* data, MemSize size);
    void Write(MemAddr address, const void* data, MemSize size);
};

}
#endif
