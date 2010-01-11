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
#ifndef __UT_IO_H
#define __UT_IO_H

/* this should be called once at startup */
thread void utsys_io_init();

/* library threads */
thread void utsys_io_write_bytes(int n, const char *bytes);
thread void utsys_io_read_bytes(shared int n, char *bytes);

/* possibly often used construct */
#define UTSYS_IO_WRITE(N, Bytes) do {			      \
    family __f;						      \
    int __n = (N);					      \
    const char *__bytes = (Bytes);			      \
    create(__f;;1;1;1;0;) utsys_io_write_bytes(__n, __bytes); \
    sync(__f);						      \
  } while(0)

#endif
