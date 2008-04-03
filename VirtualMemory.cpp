/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009  The Microgrid Project.

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
#include "VirtualMemory.h"
#include "except.h"
using namespace std;

namespace Simulator
{

void VirtualMemory::read(MemAddr address, void* _data, MemSize size)
{
    if (size > SIZE_MAX)
    {
        throw InvalidArgumentException("Size argument too big");
    }

	size_t base   = (size_t)address & -BLOCK_SIZE;	// Base address of block containing address
	size_t offset = (size_t)address - base;			// Offset within base block of address
	char*  data   = static_cast<char*>(_data);		// Byte-aligned pointer to destination

	// Split up the request in case it traverses a block boundary
	BlockMap::iterator pos = m_blocks.begin();
	while (size > 0)
	{
		// Find or insert the block
		pos = m_blocks.insert(pos, make_pair(base, (char*)NULL));
		if (pos->second == NULL) {
			// A new element was inserted, allocate and set memory
			pos->second = new char[BLOCK_SIZE];
		}
		
		// Number of bytes to read, initially
		size_t count = min( (size_t)size, (size_t)BLOCK_SIZE);

		// Read data
		memcpy(data, pos->second + offset, count);
		size  -= count;
		data  += count;
		base  += BLOCK_SIZE;
		offset = 0;
	}
}

void VirtualMemory::write(MemAddr address, void* _data, MemSize size)
{
    if (size > SIZE_MAX)
    {
        throw InvalidArgumentException("Size argument too big");
    }

	size_t base   = (size_t)address & -BLOCK_SIZE;	// Base address of block containing address
	size_t offset = (size_t)address - base;			// Offset within base block of address
	char*  data   = static_cast<char*>(_data);		// Byte-aligned pointer to destination

	// Split up the request in case it traverses a block boundary
	BlockMap::iterator pos = m_blocks.begin();
	while (size > 0)
	{
		// Find or insert the block
		pos = m_blocks.insert(pos, make_pair(base, (char*)NULL));
		if (pos->second == NULL) {
			// A new element was inserted, allocate and set memory
			pos->second = new char[BLOCK_SIZE];
		}
		
		// Number of bytes to read, initially
		size_t count = min( (size_t)size, (size_t)BLOCK_SIZE);

		// Read data
		memcpy(pos->second + offset, data, count);
		size  -= count;
		data  += count;
		base  += BLOCK_SIZE;
		offset = 0;
	}

}

VirtualMemory::~VirtualMemory()
{
	// Clean up all allocated memory
	for (BlockMap::const_iterator p = m_blocks.begin(); p != m_blocks.end(); p++)
	{
		delete[] p->second;
	}
}

}