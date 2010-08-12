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
#include <queue>

namespace MemSim
{

class CacheL2TOK;
class ST_request;

class ProcessorTOK : public LinkMGS
{
    CacheL2TOK&             m_cache;        // The cache we're connected to
    std::queue<ST_request*> m_responses;    // Responses from cache

public:
	ProcessorTOK(CacheL2TOK& cache);
	~ProcessorTOK();

    bool CanSendFeedback() const
    {
        return true;
    }

    bool SendFeedback(ST_request* req)
    {
        m_responses.push(req);
        return true;
    }

    // issue a new reqeuest
    void PutRequest(ST_request* req);

    // check the request and give a reply is available
    ST_request* GetReply();

	// remove the replied request
	void RemoveReply();
};

}
#endif

