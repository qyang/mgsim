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
#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"
#include <string>
#include <iostream>
#include <vector>
#ifdef USE_SDL
#include <SDL.h>
#endif

class Config;

class Display
{
    unsigned int          m_width,  m_height;
    unsigned int          m_scalex, m_scaley;
    unsigned int          m_refresh;
    std::vector<uint32_t> m_frame;
#ifdef USE_SDL
    SDL_Surface*          m_screen;
    SDL_TimerID           m_timer;

    static Uint32 TimerCallback(Uint32, void*);
#endif

    Display(const Display&);
    Display& operator=(const Display&);
public:
    Display(const Config& config);
    ~Display();

    void PutPixel(unsigned int x, unsigned int y, uint32_t data)
    {
        if (x < m_width && y < m_height) {
            m_frame[y * m_width + x] = data;
        }
    }

    void PutPixel(unsigned int offset, uint32_t data)
    {
        if (offset < m_width * m_height) {
            m_frame[offset] = data;
        }
    }
    
    void Resize(unsigned int w, unsigned int h); 
    void Dump(std::ostream&, unsigned key, const std::string& comment = std::string()) const;
    void Refresh();
    bool CheckEvents();
};

#endif
