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
#ifndef COMMANDS_H
#define COMMANDS_H

#ifdef ENABLE_MONITOR
# include "monitor.h"
#endif
#include "simreadline.h"
#include "MGSystem.h"
#include <vector>
#include <string>

void ExecuteCommand(Simulator::MGSystem& sys, 
                    const std::string& command, 
                    const std::vector<std::string>& args);

void HandleCommandLine(CommandLineReader& clr,
                       Simulator::MGSystem& sys,
#ifdef ENABLE_MONITOR
                       Monitor& mo,
#endif
                       bool &quit);


// Some public commands

void StepSystem(Simulator::MGSystem& system, Simulator::CycleNo cycles);


void PrintHelp(std::ostream&);
void PrintVersion(std::ostream&);
void PrintUsage(std::ostream& out, const char* cmd);
void PrintException(std::ostream& out, 
                    const std::exception& e);

void PrintComponents(std::ostream& out,
                     const Simulator::Object* root, 
                     const std::string& indent = "");
    
#endif
