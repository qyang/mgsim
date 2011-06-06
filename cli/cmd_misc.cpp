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
#include <iomanip>

using namespace std;

bool cmd_help(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    cout << left;
    for (const command_descriptor *p = &command_table[0]; p->prefix[0] != 0; ++p)
    {
        cout << setw(35) << setfill(' ') << p->use_format << " " << p->short_help << endl;
    }
    return false;
}


bool cmd_quit(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    cout << "Thank you. Come again!" << endl;
    return true;
}



