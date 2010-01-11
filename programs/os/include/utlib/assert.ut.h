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
#ifndef __UTLIB_ASSERT_H
#define __UTLIB_ASSERT_H

thread void __assert(const char* msg, const char *fname, int fline);

#define _ASSERT(Msg, File, Line) do {				\
    family __f;							\
    const char *__m = (Msg);					\
    const char *__file = (File);				\
    int __line = (Line);					\
    create(__f;;1;1;1;0;) __assert(__m, __file, __line);	\
    sync(__f);							\
  } while(0)

#define ASSERT(EX) do { \
    if (!(EX)) _ASSERT(#EX, __FILE__, __LINE__); \
  } while(0);

#endif
