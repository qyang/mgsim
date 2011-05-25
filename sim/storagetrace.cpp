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
#include "storagetrace.h"
#include "storage.h"
#include <iostream>

namespace Simulator
{

std::ostream& operator<<(std::ostream& os, const StorageTrace& st)
{
    for (size_t i = 0; i < st.m_storages.size(); ++i)
    {
        if (i != 0)
            os << ", ";
        os << st.m_storages[i]->GetFQN();
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const StorageTraceSet& sts)
{
    if (sts.m_storages.empty())
    {
        os << "(no traces)" << std::endl;
    }
    else
    {
        for (std::set<StorageTrace>::const_iterator i = sts.m_storages.begin(); i != sts.m_storages.end(); ++i)
        {
            os << "- " << *i << std::endl;
        }
    }
    return os;
}


}

