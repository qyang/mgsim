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
#include <sstream>

using namespace std;

bool DoCommand(vector<string>& args, cli_context& ctx)
{
    bool match = false;
    bool quit = false;
    stringstream backup;
    backup.copyfmt(cout);

    for (const command_descriptor *p = &command_table[0]; p->prefix[0] != 0; ++p)
    {
        // First check that the start of the command matches.

        size_t i;
        match = true;
        for (i = 0; p->prefix[i] != 0; ++i)
        {
            if (i >= args.size() || args[i] != p->prefix[i])
            {
                match = false;
                break;
            }
        }
        
        if (!match)
            continue;

        // We have a match. Check the number of arguments.
        // here i points to the first non-command argument
        size_t n_remaining_args = args.size() - i;
        if (n_remaining_args < p->min_args)
            continue;
        if (p->max_args >= 0 && n_remaining_args > p->max_args)
            continue;

        // We have a match for both the command and the arguments.
        // Get the command separately.
        vector<string> command(args.begin(), args.begin() + i);
        args.erase(args.begin(), args.begin() + i);

        quit = p->handler(command, args, ctx);
        cout << endl;
        break;
    }
    cout.copyfmt(backup);
    
    if (!match)
        cout << "Unknown command." << endl;
    return quit;
}


