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
#include "config.h"
#include "except.h"
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace Simulator;
using namespace std;

string Config::getString(string name, const string& def) const
{
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    ConfigMap::const_iterator p = m_data.find(name);
    if (p != m_data.end())
    {
        return p->second;
    }
    return def;
}

Config::Config(const string& filename)
{
    enum State
    {
        STATE_BEGIN,
        STATE_COMMENT,
        STATE_NAME,
        STATE_EQUALS,
        STATE_VALUE,
    };

    State state = STATE_BEGIN;
    string name;
    string value;

    ifstream input(filename.c_str());
    if (!input.is_open())
    {
        throw FileNotFoundException(filename);
    }

    while (!input.eof())
    {
        char c = input.get();
		if (input.fail())
		{
			break;
		}

        if (state == STATE_BEGIN && !isspace(c))
        {
            if (c == '#' || c == ';')
            {
                state = STATE_COMMENT;
            }
            else if (isalpha(c))
            {
                state = STATE_NAME;
                name = c;
            }
        }
        else if (state == STATE_COMMENT)
        {
            if (c == '\n')
            {
                state = STATE_BEGIN;
            }
        }
        else if (state == STATE_NAME)
        {
            if (isalnum(c)) name += c;
            else 
            {
                transform(name.begin(), name.end(), name.begin(), ::toupper);
                state = STATE_EQUALS;
            }
        }
        
        if (state == STATE_EQUALS && !isspace(c))
        {
            if (c == '=') state = STATE_VALUE;
        }
        else if (state == STATE_VALUE)
        {
            if (isspace(c) && value.empty())
            {
            }
            else if (!isspace(c))
            {
                value = value + c;
            }
            else 
            {
                if (value != "")
                {
                    m_data.insert(make_pair(name,value));
                    name.clear();
                    value.clear();
                }
                state = STATE_BEGIN;
            }
        }
    }
    
    if (value != "")
    {
        m_data.insert(make_pair(name,value));
    }
}

