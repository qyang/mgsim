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

void SensitiveStorage::Notify()
{
    m_kernel.ActivateProcess(m_handle);
}

void SensitiveStorage::Unnotify()
{
    if (--m_handle->activations == 0)
    {
        // Remove the handle node from the list
        *m_handle->pPrev = m_handle->next;
        if (m_handle->next != NULL) {
            m_handle->next->pPrev = m_handle->pPrev;
        }
        
        m_handle->state = STATE_IDLE;
    }
}

SensitiveStorage::SensitiveStorage(Kernel& kernel, IComponent& component, int state)
    : Storage(kernel)
{
    m_component = &component;
    m_state     = state;
}

}
