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
#ifndef PROFILE_H
#define PROFILE_H

#include "types.h"
#include <string>
#include <map>

namespace Simulator
{

#ifdef PROFILE
void PROFILE_BEGIN(const std::string& name);
void PROFILE_END(const std::string& name);
static bool inline ProfilingEnabled() { return true; }
#else
static void inline PROFILE_BEGIN(const std::string& name) {}
static void inline PROFILE_END(const std::string& name) {}
static bool inline ProfilingEnabled() { return false; }
#endif

typedef std::map<std::string, uint64_t> ProfileMap;

uint64_t GetProfileTime(const std::string& name);
const ProfileMap& GetProfiles();

}
#endif

