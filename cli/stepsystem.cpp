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
#include "commands.h"
#include "arch/dev/Selector.h"
#include <csignal>
#include <fcntl.h>

/// The currently active system, for the signal handler
static Simulator::MGSystem* active_system = NULL;

static void sigabrt_handler(int)
{
    if (active_system != NULL)
    {
        active_system->Abort();
        active_system = NULL;
    }
}

void StepSystem(Simulator::MGSystem& system, Simulator::CycleNo cycles)
{
    // This function "protects" the interactive mode by changing
    // SIGINT (^C) into simulation exceptions and ensuring that
    // stdout/stderr have sane flags when the prompt is in use.

    active_system = &system;

    struct sigaction new_handler, old_handler;
    new_handler.sa_handler = sigabrt_handler;
    new_handler.sa_flags   = 0;
    sigemptyset(&new_handler.sa_mask);
    sigaction(SIGINT, &new_handler, &old_handler);

    // The selector sets/resets O_NONBLOCK on all monitored fds.
    Simulator::Selector::GetSelector().Enable();

    try
    {
        system.Step(cycles);
    }
    catch (...)
    {
        sigaction(SIGINT, &old_handler, NULL);
        active_system = NULL;
        throw;
    }
    sigaction(SIGINT, &old_handler, NULL);
    active_system = NULL;

    Simulator::Selector::GetSelector().Disable();
}

