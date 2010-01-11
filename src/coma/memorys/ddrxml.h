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
#ifndef _DDRXML_H
#define _DDRXML_H

#include <vector>
#include <map>
using namespace std;

typedef struct _ddr_interface_t{
    int id;
    string  name;

    int tAL;     // Additive Latency  (RL = AL + CL; WL = AL + CWL), assume CWL == CL
    int tCL;     // /CAS low to valid data out (equivalent to tCAC)
    int tCWL;    // Write delay corresponding to CL
    int tRCD;    // /RAS low to /CAS low time
    int tRP;     // /RAS precharge time (minimum precharge to active time)
    int tRAS;    // Row active time (minimum active to precharge time)

    // configuration
    int nChannel;            // number of channels
    int nModeChannel;        // Mode running on multiple channels
                                      // 0 : multiple/single individual channel
                                      // 1 : merge to wider datapath

    int nRankBits;           // lg number of ranks on DIMM (only one active per DIMM)
    int nBankBits;           // lg number of banks
    int nRowBits;            // lg number of rows
    int nColumnBits;         // lg number of columns
    int nCellSizeBits;       // lg number of cell size, normally x4, x8, x16
    int nDevicePerRank;      // devices per rank
    int nDataPathBits;       // single channel datapath bits, 64 for ddr3
                             // data_path_bit == cell_size * devices_per_rank
    int nBurstLength;
} ddr_interface;

// NULL if no interfaces are load
map<int, ddr_interface*>* loadxmlddrinterfaces(const char* fname);

map<int, vector<int> >* loadxmlddrconfigurations(map<int, ddr_interface*>* pinterfaces, const char* fname);

#endif

