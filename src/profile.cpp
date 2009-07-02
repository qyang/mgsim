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
#include "profile.h"
#include "simtime.h"
using namespace std;

static ProfileMap g_TotalProfiles;
static ProfileMap g_ActiveProfiles;

#ifdef PROFILE
void PROFILE_BEGIN(const string& name)
{
    g_ActiveProfiles[name] = GetTime();
}

void PROFILE_END(const string& name)
{
    ProfileMap::const_iterator p = g_ActiveProfiles.find(name);
    if (p != g_ActiveProfiles.end())
    {
        uint64_t t = GetTime() - p->second;
        ProfileMap::iterator p = g_TotalProfiles.find(name);
        if (p != g_TotalProfiles.end()) {
            p->second += t;
        } else {
            g_TotalProfiles.insert(make_pair(name, t));
        }
    }
}
#endif

uint64_t GetProfileTime(const string& name)
{
    ProfileMap::const_iterator p = g_TotalProfiles.find(name);
    return (p != g_TotalProfiles.end()) ? p->second : 0;
}

const ProfileMap& GetProfiles()
{
    return g_TotalProfiles;
}
