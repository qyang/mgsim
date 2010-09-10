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
#include "processortok.h"
#include "cachel2tok.h"
#include "predef.h"

namespace MemSim
{

// Issue a new request from the processor
void ProcessorTOK::PutRequest(ST_request* req)
{
    // Make sure the size matches
    assert(req->offset + req->nsize <= g_nCacheLineSize);
    assert(req->nsize <= ST_request::s_nRequestAlignedSize);

    req->Conform2BitVecFormat();
    
    m_cache.m_requests.push(req);
}

// check the request and give a reply is available
ST_request* ProcessorTOK::GetReply()
{
    if (m_responses.empty())
    {
        return NULL;
    }
	return m_responses.front();
}

void ProcessorTOK::RemoveReply()
{
    assert(!m_responses.empty());
    ST_request* req = m_responses.front();
    if (--req->refcount == 0)
    {
        delete req;
    }
    m_responses.pop();
    m_popped = true;
}

ProcessorTOK::ProcessorTOK(CacheL2TOK& cache)
  : m_cache(cache), m_pushed(false)
{
    m_cache.RegisterProcessor(*this);
}

ProcessorTOK::~ProcessorTOK()
{
    m_cache.UnregisterProcessor(*this);
}

}
