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
#ifndef _NETWORK_NODE_H
#define _NETWORK_NODE_H

#include "predef.h"
#include <queue>

namespace MemSim
{

class Network_Node
{
	std::queue<Message*> m_fifoinNetwork;   // Input buffer from previous node
	std::queue<Message*> m_fifooutNetwork;  // Output buffer for next node
	Network_Node*           m_next;
	
public:
	void BehaviorNode()
	{
	    // Send outgoing requests to next node
	    if (!m_fifooutNetwork.empty())
	    {
            m_next->m_fifoinNetwork.push(m_fifooutNetwork.front());
	        m_fifooutNetwork.pop();
        }
	}
	
	bool SendRequest(Message* req)
	{
	    m_fifooutNetwork.push(req);
	    return true;
	}
	
	Message* ReceiveRequest()
	{
	    if (m_fifoinNetwork.empty())
	    {
	        return NULL;
	    }
	    Message* req = m_fifoinNetwork.front();
	    m_fifoinNetwork.pop();
	    return req;
	}
	
	Network_Node()//sc_module_name nm, sc_clock& clock)
	  : /*sc_module(nm),*/ m_next(NULL)
	{
		//SC_METHOD(NetworkNode::BehaviorIni);
		//sensitive << clock.negedge_event();
		//dont_initialize();
	}

	void SetNext(Network_Node* next)
	{
	    assert(m_next == NULL);
	    m_next = next;
	}	
};

}
#endif
