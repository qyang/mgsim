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

bool cmd_line(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = args[0];
    args.erase(args.begin());
    DoObjectCommand<Inspect::Line>(cout, ctx.sys, pat, args);
    return false;
}

bool cmd_info(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = args[0];
    args.erase(args.begin());
    DoObjectCommand<Inspect::Info>(cout, ctx.sys, pat, args);
    return false;
}

bool cmd_inspect(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    string pat = args[0];
    args.erase(args.begin());

    if (args.empty())
        ReadSampleVariables(cout, pat);
    DoObjectCommand<Inspect::Read>(cout, ctx.sys, pat, args);    
    return false;
}

