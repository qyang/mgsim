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
#define __UTLIB_STRING_H
#define __UTLIB_STRING_H

thread void strlen(shared int n, const char *str);
thread void strcat(shared char *str, const char *s2);

thread void str_print_int(shared char *str, int n);

/* possibly often used */

#define UTIO_PUTS(String) do {				\
    family __f;						\
    int __strlen;					\
    const char *__str = (String);			\
    create(__f;;1;1;1;0;) strlen(__strlen, __str);	\
    sync(__f);						\
    create(__f;;1;1;1;0;) utsys_io_write_bytes(__strlen, __str); \
    sync(__f);							 \
  } while(0)

#endif
