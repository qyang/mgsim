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

static string Trim(const string& str)
{
    size_t beg = str.find_first_not_of(" \t");
    if (beg != string::npos)
    {
        size_t end = str.find_last_not_of(" \t");
        return str.substr(beg, end - beg + 1);
    }
    return "";
}

static vector<string> prevCommands;

bool HandleCommandLine(cli_context& ctx)
{
    stringstream prompt;
    prompt << dec << setw(8) << setfill('0') << right << ctx.sys.GetKernel().GetCycleNo() << "> ";

    // Read the command line and split into commands
    char* line = ctx.clr.GetCommandLine(prompt.str());
    if (line == NULL)
    {
        // End of input
        cout << endl;
        return true;
    }

    vector<string> commands = Tokenize(line, ";");
    if (commands.size() == 0)
    {
        // Empty line, use previous command line
        commands = prevCommands;
    }
    prevCommands = commands;
    free(line);

    bool quit = false;
    // Execute all commands
    for (vector<string>::const_iterator command = commands.begin();
         command != commands.end() && !quit;
         ++command)
    {
        vector<string> args = Tokenize(Trim(*command), " ");
        if (args.empty())
            /* empty command */
            continue;

        // Substitute any aliases
        ExpandAliases(args);

        // Then perform the command.
        quit = DoCommand(args, ctx);
    }

    return quit;
}

