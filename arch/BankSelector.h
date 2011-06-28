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
#ifndef BANK_SELECTION_H
#define BANK_SELECTION_H

#include "sim/kernel.h"
#include "simtypes.h"

namespace Simulator 
{

    class IBankSelector {
    public:

        // cache line address to cache tag + bank index
        virtual void Map(MemAddr address, MemAddr& tag, size_t& index) = 0;
        // cache tag + bank index to cache line address
        virtual MemAddr Unmap(MemAddr tag, size_t index) = 0;

        virtual std::string GetName() const = 0;
        virtual size_t GetNumBanks() const = 0;
        ~IBankSelector() {};

        static IBankSelector* makeSelector(Object& parent, const std::string& name, size_t numBanks);
    };


}


#endif
