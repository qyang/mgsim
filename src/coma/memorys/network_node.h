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
#ifndef _NETWORK_NODE_H
#define _NETWORK_NODE_H

#include "predef.h"
#include "network_if.h"

namespace MemSim
{

class Network_Node : public sc_module
{
public:
	// port for input and output
	sc_fifo<ST_request*> m_fifoIn;

	sc_fifo<ST_request*> &m_fifoNetIn;
	sc_fifo_out<ST_request*> m_fifoNetOut;

private:
    ST_request* m_pReqCurINI;

	void BehaviorIni()
	{
        if (m_pReqCurINI == NULL)
        {
            // Get a request
            if (m_fifoIn.num_available_fast() > 0)
            {
                m_fifoIn.nb_read(m_pReqCurINI);
            }
        }

        if (m_pReqCurINI != NULL)
        {
            // Forward transaction to the next node
            if (m_fifoNetOut.nb_write(m_pReqCurINI))
            {
                m_pReqCurINI = NULL;
            }
        }
	}
   
public:
	SC_HAS_PROCESS(Network_Node);
	
	Network_Node(sc_module_name nm, sc_clock& clock, sc_fifo<ST_request*>& fifonetin, sc_fifo_out<ST_request*>& fifonetout)
	  : sc_module(nm),
	    m_fifoNetIn(fifonetin),
	    m_pReqCurINI(NULL)
	{
		// process for requests from CPU
		SC_METHOD(BehaviorIni);
		sensitive << clock.negedge_event();
		dont_initialize();
		
		fifonetout(m_fifoIn);
	}
};

}
#endif
