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
#ifndef _MEMBANKST_IF_H
#define _MEMBANKST_IF_H

#include "predef.h"

namespace MemSim{
//{ memory simulator namespace
//////////////////////////////


class MemBank_if : public sc_interface
{

public:
	sc_event m_eFeedbackEvent;

	// Slave interface
	/*virtual bool request(ST_request *req) = 0;*/
	//	virtual bool read(ST_request *req) = 0;
	//	virtual bool write(ST_request *req) = 0;
};

//////////////////////////////
//} memory simulator namespace
}

#endif
