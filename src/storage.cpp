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
#include "storage.h"

namespace Simulator
{

void Storage::Initialize(Kernel::StorageInfo* handle)
{
    assert(m_handle == NULL);
    assert(handle != NULL);
    m_handle = handle;

    for (std::vector<ProcessInfo>::iterator p = m_processes.begin(); p != m_processes.end(); ++p)
    {
        p->m_handle = m_kernel.GetProcessInfo(p->m_component, p->m_state);
        assert(p->m_handle != NULL);
    }
    m_initialized = true;
}

void Storage::Notify()
{
    // Activate all sensitive processes
    for (std::vector<ProcessInfo>::const_iterator p = m_processes.begin(); p != m_processes.end(); ++p)
    {
        m_kernel.ActivateProcess(p->m_handle);
    }
}

void Storage::Unnotify()
{
    // Deactivate all sensitive processes
    for (std::vector<ProcessInfo>::const_iterator p = m_processes.begin(); p != m_processes.end(); ++p)
    {
        // A process can be sensitive to multiple objects, so we only remove it from the list
        // if the count becomes zero
        if (--p->m_handle->activations == 0)
        {
            // Remove the handle node from the list
            *p->m_handle->pPrev = p->m_handle->next;
            if (p->m_handle->next != NULL) {
                p->m_handle->next->pPrev = p->m_handle->pPrev;
            }
        
            p->m_handle->state = STATE_IDLE;
        }
    }
}

void Storage::Sensitive(IComponent& component, int state)
{
    assert(!m_initialized);
    
    ProcessInfo pi;
    pi.m_component = &component;
    pi.m_state     = state;
    m_processes.push_back(pi);
}

}
