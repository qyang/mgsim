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
#include "traceloader.h"
using namespace MemSim;

// return the number of processors
UINT traceloader(TraceFile* &pTrf, char* strName)
{
    pTrf = new TraceFile(strName);

    TraceFile::OpenResult nTrfOpenRes = pTrf->open();

    if(nTrfOpenRes != TraceFile::OPEN_RES_OK) {
        cerr << "Error opening trace file. Error Code = "
            << nTrfOpenRes << endl;

        exit(1);
    }

    // get processor number
    return pTrf->getProcCnt();
}
