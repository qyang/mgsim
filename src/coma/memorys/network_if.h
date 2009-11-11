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
#ifndef _NETWORK_IF_H
#define _NETWORK_IF_H

#include "predef.h"

namespace MemSim{
//{ memory simulator namespace
//////////////////////////////


class Network_if : public sc_interface
{
public:
	sc_fifo<ST_request*> m_fifoinNetwork;
	sc_fifo_out<ST_request*> m_fifooutNetwork;
	sc_out<bool> port_net_forward;

protected:
	bool m_bBelow;	// avoid runtime judgment of the class type

public:
	// sc_event m_eFeedbackEvent;

	// Slave interface
	virtual bool RequestNetwork(ST_request *req)
	{
		return (m_fifooutNetwork->nb_write(req));
	}

	// decide whether the request can be directly forwarded to the next node

	virtual bool DirectForward(ST_request* req){return false;};
	virtual bool MayHandle(ST_request* req) = 0;

	bool IsBelowIF(){return m_bBelow;}

};

//////////////////////////////
//} memory simulator namespace
}

#endif
