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
#include "kernel.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

namespace Simulator
{

static string ConstructErrorMessage(const Object& obj, const string& message)
{
    string name = obj.getFQN();
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    
    stringstream ss;
    ss << "[" << setw(8) << setfill('0') << obj.getKernel()->getCycleNo() << ":" << name << "] " << message;
    return ss.str();
}

SimulationException::SimulationException(const Object& obj, const string& message)
    : runtime_error(ConstructErrorMessage(obj, message))
{
}

}
