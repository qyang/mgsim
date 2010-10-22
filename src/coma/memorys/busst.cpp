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
#include "busst.h"
#include "busst_master.h"
using namespace MemSim;

void BusST::BehaviorMaster()
{
    if (m_pReqCurINI == NULL)
    {
        // check if there is any request available from master side
        if (m_pfifoMasterIn.num_available_fast() > 0)
		{
            m_pfifoMasterIn.nb_read(m_pReqCurINI);
        }
    }
    
    if (m_pReqCurINI != NULL)
    {
	    // Dispatch request
        // If failed, stall and try again next cycle
    	if (m_port_slave.request(m_pReqCurINI))
        {
            m_pReqCurINI = NULL;
	    }
	}
}

void BusST::BehaviorSlave()
{
    switch (m_nStateMEM)
    {
    // when the bus is free
    case STATE_MEM_AVAILABLE:
    {
        // check if there is any requests available from the slave side
	    sc_fifo<MemSim::ST_request*> * p = static_cast<sc_fifo<MemSim::ST_request*>* >(m_port_fifo_slave_in.get_interface_fast());
        if (p->num_available_fast() > 0 &&
	        m_port_fifo_slave_in.nb_read(m_pReqCurMEM))
        {
            if ((m_pReqCurMEM->type == MemoryState::REQUEST_INVALIDATE_BR)||( (m_pReqCurMEM->type == MemoryState::REQUEST_WRITE_REPLY)&&(m_pReqCurMEM->bbackinv) ))
            {
                m_vecBCMasters = m_vecBusMaster;
                m_pReqCurMEM->refcount = m_vecBCMasters.size();
                BroadCastFeedback(m_pReqCurMEM);
            }
            else
            {
                // send the feedback to master
                SendFeedbackToMaster(m_pReqCurMEM);
            }
        }
        break;
    }
    
    case STATE_MEM_CONGEST:
        if (m_pReqCurMEM->type == MemoryState::REQUEST_INVALIDATE_BR ||
           (m_pReqCurMEM->type == MemoryState::REQUEST_WRITE_REPLY && m_pReqCurMEM->bbackinv))
        {
            BroadCastFeedback(m_pReqCurMEM);
        }
        else
        {
            SendFeedbackToMaster(m_pReqCurMEM);
        }
        break;

    default:
        break;
    }
}

void BusST::SendFeedbackToMaster(ST_request* req)
{
    if ( !get_initiator(req)->GetBusMaster()->GetFeedback(req) )
    {
        m_nStateMEM = STATE_MEM_CONGEST;
        return;
    }

    // pop when succeed
    pop_initiator(req);

    m_nStateMEM = STATE_MEM_AVAILABLE;
}


// CHKS : for WR request, this maybe unsafe, 
// should let all the WR(IB) go back first before WR reply to the initiator
void BusST::BroadCastFeedback(ST_request* req)
{
    for (vector<BusST_Master*>::iterator iter = m_vecBCMasters.begin(); iter != m_vecBCMasters.end(); ++iter)
    {
        if ((*iter)->GetFeedback(req))
        {
            m_vecBCMasters.erase(iter);
            iter = m_vecBCMasters.begin();
            if (iter == m_vecBCMasters.end())
            {
                break;
            }
        }
    }
    
    m_nStateMEM = (m_vecBCMasters.empty()) ? STATE_MEM_AVAILABLE : STATE_MEM_CONGEST;
}

bool BusST::request(ST_request *req)
{
    return m_pfifoMasterIn.nb_write(req);
}
