/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011,2012  The Microgrid Project.

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

void ExpandAliases(vector<string>& args)
{
    string command = args[0];
    args.erase(args.begin());

    for (const command_alias *p = &alias_table[0]; p->alias != 0; ++p)
    {
        if (p->alias == command)
        {
            size_t i;
            for (i = 0; p->subst[i] != 0; ++i) ;
            args.insert(args.begin(), &p->subst[0], &p->subst[i]);
            return;
        }
    }

    // No alias found, insert command back at beginning.
    args.insert(args.begin(), command);
}

bool cmd_aliases(const vector<string>& command, vector<string>& args, cli_context& ctx)
{
    cout << "Aliases:" << endl;
    for (const command_alias *p = &alias_table[0]; p->alias != 0; ++p)
    {
        cout << setw(10) << p->alias;
        for (size_t i = 0; p->subst[i] != 0; ++i)
            cout << ' ' << p->subst[i];
        cout << endl;
    }
    cout << endl;
    return false;
}


