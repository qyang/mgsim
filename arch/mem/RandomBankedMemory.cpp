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
#include "RandomBankedMemory.h"
#include <iostream>

namespace Simulator
{

size_t RandomBankedMemory::GetBankFromAddress(MemAddr address) const
{
    address &= ~(m_cachelineSize - 1);
#if MEMSIZE_MAX >= 4294967296
    address = address ^ ((address >> 32) | (address << (sizeof(address)*8-32)));
#endif
    address = address ^ ((address >> 16) | (address << (sizeof(address)*8-16)));
    address = address ^ ((address >> 8) | (address << (sizeof(address)*8-8)));
    address = address ^ ((address >> 4) | (address << (sizeof(address)*8-4)));
    return address % m_banks.size();
}

RandomBankedMemory::RandomBankedMemory(const std::string& name, Object& parent, Clock& clock, Config& config)
    : BankedMemory(name, parent, clock, config)
{
}

void RandomBankedMemory::Cmd_Info(std::ostream& out, const std::vector<std::string>& /*arguments*/) const
{
    out <<
    "The Random Banked Memory represents a switched memory network between P\n"
    "processors and N memory banks. Requests are sequentialized on each bank and the\n"
    "cache line-to-bank mapping uses a hash function to pseudo-randomize the mapping\n"
    "to avoid structural bank conflicts.\n\n"
    "Supported operations:\n"
    "- info <component>\n"
    "  Displays the currently reserved and allocated memory ranges\n\n"
    "- inspect <component> <start> <size>\n"
    "  Reads the specified number of bytes of raw data from memory from the\n"
    "  specified address\n\n"
    "- inspect <component> requests\n"
    "  Reads the banks' requests buffers and queues\n";
}

}
