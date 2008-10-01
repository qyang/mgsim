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
#ifndef __UT_SWITCHBOARD_H
#define __UT_SWITCHBOARD_H

/* the following can be called by any thread
   in a family of 1 thread to perform communication
   with the switchboard.
*/
thread void utsys_sb_exchange(shared int service, shared void* p);

/* helper for commonly used construct */
#define UTSYS_SB_SEND(Code, Pointer) do {			\
    family __f;							\
    register int __code = (Code);				\
    register void *__ptr = (void*)(Pointer);			\
    create(__f;;1;1;1;0;) utsys_sb_exchange(__code, __ptr);	\
    sync(__f);							\
  } while(0)

#define UTSYS_SB_EXCHANGE(CodeVar, PointerVar) do {			\
    family __f;								\
    create(__f;;1;1;1;0;) utsys_sb_exchange(CodeVar, PointerVar);	\
    sync(__f);								\
  } while(0)

/* the following thread should be started by
   the boot code in a family of 1 thread */
thread void utsys_sb_manager();




/* predefined switchboard codes */
#define _SB_NONE 0
#define _SB_REGISTER_SERVICE 1
#define _SB_MEM_ALLOC 2
#define _SB_MEM_FREE 3
#define _SB_IO_WRSTRING 4
#define _SB_IO_RDSTRING 5


#endif
