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
#ifndef _PROCESSORTOK_H
#define _PROCESSORTOK_H

#include "../simlink/linkmgs.h"
#include "../../storage.h"
#include <queue>

namespace MemSim
{

class CacheL2TOK;
class Message;

class ProcessorTOK : public LinkMGS
{
    CacheL2TOK&             m_cache;        // The cache we're connected to
    std::queue<Message*> m_responses;    // Responses from cache
    bool                    m_pushed;
    bool                    m_popped;

public:
    Simulator::SingleFlag*  m_active;

	ProcessorTOK(CacheL2TOK& cache);
	~ProcessorTOK();
	
	void OnCycleStart()
	{
	    m_pushed = false;
	}
	
	void OnCycleEnd()
	{
	    if (m_responses.empty() && m_popped && !m_pushed)
	    {
	        m_active->Clear();
	    }
	    m_popped = false;
	}

    bool CanSendFeedback() const
    {
        return true;
    }

    bool SendFeedback(Message* req)
    {
        m_responses.push(req);
        if (!m_pushed)
        {
            m_active->Set();
            m_pushed = true;
        }
        return true;
    }

    // issue a new reqeuest
    void PutRequest(Message* req);
    
    size_t NumReplies() const
    {
        return m_responses.size();
    }

    // check the request and give a reply is available
    Message* GetReply();

	// remove the replied request
	void RemoveReply();
};

}
#endif

