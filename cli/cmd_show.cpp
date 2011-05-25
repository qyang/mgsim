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

using namespace Simulator;
using namespace std;


bool cmd_show_vars(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = "*";
    if (!args.empty())
        pat = args[0];
    ListSampleVariables(cout, pat);
    return false;
}


bool cmd_show_syms(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = "*";
    if (!args.empty())
        pat = args[0];
    ctx.sys.GetSymTable().Write(cout, pat);
    return false;
}


bool cmd_show_components(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = "*";
    if (!args.empty())
        pat = args[0];

    size_t levels = 0;
    if (args.size() > 1)
        levels = strtoul(args[1].c_str(), 0, 0);

    ctx.sys.PrintComponents(cout, pat, levels);
    return false;
}

bool cmd_show_processes(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = "*";
    if (!args.empty())
        pat = args[0];
    ctx.sys.PrintProcesses(cout, pat);
    return false;
}


bool cmd_show_devdb(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    DeviceDatabase::GetDatabase().Print(std::cout);
    return false;
}

