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
#include "predef.h"

namespace MemSim
{

unsigned int Message::s_nRequestAlignedSize;

unsigned int g_nCacheLineSize  = 0;

int lg2(int n)
{
	int r = 0;
	while (n > 1)
	{
		r++;
		n /= 2;
	}
	return r;
}

unsigned int lg2(unsigned int n)
{
	int r = 0;
	while (n > 1)
	{
		r++;
		n /= 2;
	}
	return r;
}

unsigned int CacheState::s_nTotalToken = 0;

}
