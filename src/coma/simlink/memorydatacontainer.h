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
#ifndef _MEMORY_DATA_CONTAINER_H
#define _MEMORY_DATA_CONTAINER_H
#include "smdatatype.h"
#include "../memorys/dcswitch.h"

class MemoryDataContainer
{
private:

public:
    static MemoryDataContainer * s_pDataContainer;

    MemoryDataContainer();

	virtual ~MemoryDataContainer(){};

    // load memory image file from certain address
    virtual void LoadMemoryImage(char* imgfile, uint64_t addr) = 0;

    // load memory image file. 
    virtual void LoadMemoryImage(char* imgfile) = 0;

    // update the data from a certain address
    // return : whether the operation is successful
    // ??? permission to be defined
    virtual bool Update(uint64_t startingAddress, uint64_t size, char* data, unsigned char permission = 0) = 0;

    virtual bool UpdateAllocate(uint64_t& startingAddress, uint64_t size, unsigned char permission = 0) = 0;

    virtual bool UpdateReserve(uint64_t startingAddress, uint64_t size, unsigned char permission) = 0;

    virtual bool UpdateUnreserve(uint64_t startingAddress) = 0;

    // fetch data from the certain address
    // return : whether the operation is successful
    virtual bool Fetch(uint64_t startingAddress, uint64_t size, char* data) = 0;

#ifdef MEM_DATA_VERIFY
    // verify the validness of a piece of data
    virtual bool Verify(uint64 startingAddress, uint64_t size, char* data) = 0;

    // verify update, update for every write
    virtual bool VerifyUpdate(uint64_t startingAddress, uint64_t size, char* data) = 0;
#endif
    // print the data in memory
    // return : whether the operation is successful
    // method : 0 - normal, standalone 
    // method : 1 - one line mode
    virtual void PrintData(unsigned int method=0) = 0;
};

#endif

