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
#ifndef CLIB_EXCEPT_H
#define CLIB_EXCEPT_H

#include <string>
#include <stdexcept>

namespace Simulator
{

class Object;

// Base exception class
class SimulationException : public std::runtime_error
{
public:
    SimulationException(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidArgumentException : public std::runtime_error

{
public:
    InvalidArgumentException(const std::string& msg) : std::runtime_error(msg) {}
};

class IllegalInstructionException : public SimulationException
{
public:
    IllegalInstructionException(const Object& obj, const std::string& msg) : SimulationException(msg) {}
};

class IOException : public std::runtime_error
{
public:
    IOException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileNotFoundException : public IOException
{
public:
    FileNotFoundException(const std::string& filename) : IOException("File not found: " + filename) {}
};

class SecurityException : public SimulationException
{
public:
    SecurityException(const Object& obj, const std::string& msg) : SimulationException(msg) {}
};

}
#endif

