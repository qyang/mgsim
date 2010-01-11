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
#ifndef _BUSST_SLAVE_IF_H
#define _BUSST_SLAVE_IF_H

#include "predef.h"
#include "busst_if.h"

namespace MemSim{
//{ memory simulator namespace

class BusST_Slave_if : public sc_interface
{

public:
    sc_fifo<ST_request*> channel_fifo_slave;	// as a slave
	sc_event m_eFeedbackEvent;
    BusST_if* m_pBusST;
protected:
	sc_fifo<ST_request*> *m_pfifoReqIn;

public:

	BusST_Slave_if()
	{
		m_pfifoReqIn = new sc_fifo<ST_request*>();
	}

	~BusST_Slave_if()
	{
		delete m_pfifoReqIn;
	}

	// Slave interface
	virtual bool request(ST_request *req)
	{
		return (m_pfifoReqIn->nb_write(req));
	}

	virtual __address_t StartAddress() const = 0;
	virtual __address_t EndAddress() const = 0;
};

//} memory simulator namespace
}
#endif

