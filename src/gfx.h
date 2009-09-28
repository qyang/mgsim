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
#ifndef GFX_H
#define GFX_H

#include "types.h"
#include <cassert>
#include <cstddef>
#include <string>
#include <iostream>

class GfxFrameBuffer {
 public:
  GfxFrameBuffer(void);
  GfxFrameBuffer(size_t w, size_t h);
  ~GfxFrameBuffer(void);

  uint32_t* getBuffer(void) const { return buffer; }

  size_t getWidth(void) const { return width; }
  size_t getHeight(void) const { return height; }

  void putPixel(size_t x, size_t y, uint32_t data) const
  {
    if (x >= width || y >= height)
      return;
    buffer[x + y * width] = data;
  }

  void putPixel(size_t offset, uint32_t data) const
  {
    if (offset >= (width * height))
      return;
    buffer[offset] = data;
  }

  void resize(size_t nw, size_t nh);

  void dump(std::ostream&, unsigned key, const std::string& comment = std::string()) const;

protected:
  GfxFrameBuffer(const GfxFrameBuffer&);
  GfxFrameBuffer& operator=(const GfxFrameBuffer&);

  uint32_t *__restrict__ buffer;
  size_t width;
  size_t height;
};

class GfxScaler;
class GfxScreen;

class GfxDisplay
{
 public:
  GfxDisplay(size_t rrate, size_t w, size_t h, 
	     size_t scx, size_t scy, bool enable_output = true);
  ~GfxDisplay(void);

  const GfxFrameBuffer& getFrameBuffer(void) const { return fb; }

  void resizeFrameBuffer(size_t nw, size_t nh);
  void resizeScreen(size_t nw, size_t nh);
  
  void cycle(void) {
    if (counter++ > refreshrate) {
      counter = 0;
      refresh();
    }
  }
  
  void refresh(void);
  void checkEvents(bool skip_refresh = false);
  
 protected:
  size_t scalex;
  size_t scaley;
  size_t counter;
  size_t refreshrate;
  GfxFrameBuffer fb;
  GfxScreen *screen;
  GfxScaler *scaler;

  GfxDisplay(const GfxDisplay&);
  GfxDisplay& operator=(const GfxDisplay&);
};

#endif
