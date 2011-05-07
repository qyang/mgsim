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
#ifndef ELFLOADER_H
#define ELFLOADER_H

#include "Memory.h"
#include "ActiveROM.h"
#include <algorithm>

// Load the program file into the memory.
// Arguments:
// - ranges: the set of loadable virtual ranges in the system memory address space
// - memory: the memory administrative interface to set permissions
// - elf_image_data: raw ELF data
// - elf_image_size: size of raw ELF data
// - quiet: set to true to enable verbose reporting of the ELF loading process
std::pair<Simulator::MemAddr, bool>
    LoadProgram(std::vector<Simulator::ActiveROM::LoadableRange>& ranges, 
                Simulator::IMemoryAdmin& memory,
                char *elf_image_data,
                Simulator::MemSize elf_image_size,
                bool verbose);

#endif

