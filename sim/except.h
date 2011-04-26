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
#ifndef CLIB_EXCEPT_H
#define CLIB_EXCEPT_H

#include <string>
#include <stdexcept>
#include <list>
#include <cstdarg>
#include <cstdio>

namespace Simulator
{

class Object;

// Base exception class
class SimulationException : public std::runtime_error
{
    std::list<std::string> m_details;
public:
    const std::list<std::string>& GetDetails() const { return m_details; }
    
    void AddDetails(const std::string& msg) { m_details.push_back(msg); }
    SimulationException(const std::string& msg) : std::runtime_error(msg) {}
    SimulationException(const std::string& msg, const Object& object);
    SimulationException(const Object& object, const std::string& msg);
    virtual ~SimulationException() throw() {}
};

class InvalidArgumentException : public SimulationException
{
public:
    InvalidArgumentException(const Object& object, const std::string& msg) : SimulationException(msg, object) {}
    InvalidArgumentException(const std::string& msg) : SimulationException(msg) {}
};

class IllegalInstructionException : public SimulationException
{
public:
    IllegalInstructionException(const Object& object, const std::string& msg) : SimulationException(msg, object) {}
    IllegalInstructionException(const std::string& msg) : SimulationException(msg) {}
};

class VirtualIOException : public SimulationException
{
public:
    VirtualIOException(const Object& object, const std::string& msg) : SimulationException(msg, object) {}
    VirtualIOException(const std::string& msg) : SimulationException(msg) {}
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
    SecurityException(const Object& object, const std::string& msg) : SimulationException(msg, object) {}
};

template<typename Except>
Except exceptf(const char* fmt, ...)
#ifdef __GNUC__
__attribute__((format (printf, 1, 2)))
#endif
;

template<typename Except>
Except exceptf(const Object& obj, const char* fmt, ...)
#ifdef __GNUC__
__attribute__((format (printf, 2, 3)))
#endif
;

template<typename Except>
Except exceptf(const char* fmt, ...)
{
    va_list ap;
    char buf[1024];
    va_start(ap, fmt);
    vsnprintf(buf, 1024, fmt, ap);
    va_end(ap);
    return Except(std::string(buf));
}

template<typename Except>
Except exceptf(const Object& obj, const char* fmt, ...)
{
    va_list ap;
    char buf[1024];
    va_start(ap, fmt);
    vsnprintf(buf, 1024, fmt, ap);
    va_end(ap);
    return Except(obj, std::string(buf));
}


}
#endif

