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
#ifndef _DRV_CLK_H
#define _DRV_CLK_H

#include "predef.h"

namespace MemSim{
//{ memory simulator namespace
//////////////////////////////


class DRV_clk : public sc_module
{
public:
    sc_in<bool> port_clk;

private:
    bool m_bStep;

public:
    SC_HAS_PROCESS(DRV_clk);
    DRV_clk(sc_module_name nm, bool step = false) : sc_module(nm), m_bStep(step)
    {
        if (step)
        {
            SC_THREAD(StepBehavior);
            sensitive << port_clk.pos();
            

            SC_THREAD(StepBehavior);
            sensitive << port_clk.neg();
        }
    }

    void StepBehavior()
    {
        wait(0, SC_NS);
        sc_stop();
    }
};

//////////////////////////////
//} memory simulator namespace
}

#endif
