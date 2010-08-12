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
#ifndef __UT_MEM_H
#define __UT_MEM_H

/* call this once at program startup */
thread void utsys_mem_init();

thread void utsys_mem_alloc(shared int size, shared void *p);
thread void utsys_mem_free(shared void *p);


#define UTSYS_ALLOC(N, Pointer) do {			\
    family __f;						\
    int __sz = (N);					\
    void *__p = (Pointer);				\
    create(__f;;1;1;1;0;) utsys_mem_alloc(__sz, __p);	\
    sync(__f);						\
    (Pointer) = __p;					\
  } while(0)

#define UTSYS_FREE(Pointer) do {		\
    family __f;					\
    void *__p = (Pointer);			\
    create(__f;;1;1;1;0;) utsys_mem_free(__p);	\
    sync(__f);					\
  } while(0)


#define

