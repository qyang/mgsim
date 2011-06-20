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
#ifndef RPC_UNIX_H
#define RPC_UNIX_H

#include "RPC.h"
#include "sim/inspect.h"
#include <vector>
#include <dirent.h>

namespace Simulator
{
    class UnixInterface : public Object, public IRPCServiceProvider, public Inspect::Interface<Inspect::Info>
    {      
        typedef int       HostFD;
        typedef size_t    VirtualFD;

        struct VirtualDescriptor
        {
            bool        active;
            HostFD      hfd;
            DIR         *dir;
            CycleNo     cycle_open;
            CycleNo     cycle_use;
            std::string fname;
        };

        std::vector<VirtualDescriptor> m_vfds;

        VirtualDescriptor* GetEntry(VirtualFD vfd);
        VirtualFD GetNewVFD(const std::string& fname, HostFD new_hfd);
        VirtualFD DuplicateVFD(VirtualFD original, HostFD new_hfd);
        VirtualDescriptor* DuplicateVFD2(VirtualFD original, VirtualFD target);

        // statistics
        uint64_t m_nrequests;
        uint64_t m_nfailures;
        uint64_t m_nstats;
        uint64_t m_nreads;
        uint64_t m_nread_bytes;
        uint64_t m_nwrites;
        uint64_t m_nwrite_bytes;

    public:
        
        UnixInterface(const std::string& name, Object& parent);

        void Service(uint32_t procedure_id,
                     std::vector<uint32_t>& res1, size_t res1_maxsize,
                     std::vector<uint32_t>& res2, size_t res2_maxsize,
                     const std::vector<uint32_t>& arg1, 
                     const std::vector<uint32_t>& arg2, 
                     uint32_t arg3, uint32_t arg4);
        
        std::string GetName() { return GetFQN(); }

        void Cmd_Info(std::ostream& out, const std::vector<std::string>& /*args*/) const;

    };


}


#endif
