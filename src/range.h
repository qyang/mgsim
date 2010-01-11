/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010  The Microgrid Project.

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
#ifndef RANGE_H
#define RANGE_H

#include <set>
#include <string>
#include <sstream>

template <typename T>
std::set<T> parse_range(const std::string& str, const T& first, const T& end)
{
    // Read the range
    std::set<T> elems;
    std::stringstream is(str);
    std::string s;
    while (std::getline(is, s, ',')) {
        if (s == "all") {
            for (T i = first; i != end; ++i) {
                elems.insert(i);
            }
        } else {
            std::stringstream is2(s);
            T a(end), last(end); --last;
            is2 >> a;
            if (a >= first && a < end) {
                T b(a);
                if (is2.get() == '-') {
                    is2 >> b;
                    b = std::min<T>(b, last);
                }
                b = std::max<T>(a,b);
                for (T i = a; i <= b; ++i) {
                    elems.insert(i);
                }
            }
        }
    }
    return elems;
}

#endif
