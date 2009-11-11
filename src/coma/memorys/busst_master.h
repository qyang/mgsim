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
#ifndef _BUSST_MASTER_H
#define _BUSST_MASTER_H

#include "predef.h"

#include "busst_if.h"
#include "simcontrol.h"

namespace MemSim{
//{ memory simulator namespace

class BusST_Master : virtual public SimObj
{
public:
	// ports
	sc_port<BusST_if> port_bus;

	sc_fifo<ST_request*> *m_pfifoFeedback;

private:

	sc_event m_eBusFeedback;

public:
    BusST_Master()
    {
        SetBusMaster(this);
    }

	sc_event* GetBusFeedbackEvent()
	{
		return &m_eBusFeedback;
	}

	bool GetFeedback(ST_request* req)
	{
        assert(m_pfifoFeedback != NULL);
		return (m_pfifoFeedback->nb_write(req));
	}

	void NotifyBusFeedbackEvent()
	{
		m_eBusFeedback.notify();
LOG_VERBOSE_BEGIN(VERBOSE_ALL)
		clog << "bus notified \n";
LOG_VERBOSE_END
	}

};

//} memory simulator namespace
}
#endif

