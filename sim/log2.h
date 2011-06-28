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
#ifndef LOG2_H
#define LOG2_H

template <typename T>
#ifdef __GNUC__
__attribute__((const))
#endif
static bool IsPowerOfTwo(const T& x)
{
    return (x & (x - 1)) == 0;
}

template<typename T>
#ifdef __GNUC__
__attribute__((const))
#endif
static inline unsigned ilog2(T n) 
{
    // returns the first power of two equal to
    // or greater than n. For example ilog2(1) = 0, ilog2(4) = 2, ilog2(5) = 3
    unsigned l = 0;
    T r = 1;
    while (r < n)
    {
        l++;
        r *= 2;
    }
    return l;
}

#endif
