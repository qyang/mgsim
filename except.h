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

namespace Simulator
{

// Base exception class
class Exception
{
    const std::string message;
public:
    virtual Exception*  clone()   { return new Exception( *this ); }
    virtual void        raise()   { throw Exception( *this ); }
    virtual const char* getType() { return "Exception"; }

    const std::string& getMessage()
    {
        return message;
    }

    Exception( const std::string& msg ) : message(msg) {}
    Exception() {}
    virtual ~Exception() {}
};

#define EXCEPTION_COMMON(name) \
    Exception*  clone()   { return new name( *this ); } \
    void        raise()   { throw name( *this ); } \
    const char* getType() { return #name; }

// Out of memory, thrown by new
class OutOfMemoryException : public Exception
{
public:
    EXCEPTION_COMMON(OutOfMemoryException)

    // Only has this constructor
    OutOfMemoryException() {}
};

// Out of range, thrown by string class on invalid index
class OutOfRangeException : public Exception
{
public:
    EXCEPTION_COMMON(OutOfRangeException)

    OutOfRangeException( const std::string& msg ) : Exception(msg) {}
    OutOfRangeException() {}
};

// Format exception, thrown by FormatString class
class FormatException : public Exception
{
public:
    EXCEPTION_COMMON(FormatException)

    FormatException( const std::string& msg ) : Exception(msg) {}
    FormatException() {}
};

class ThreadException : public Exception
{
public:
    EXCEPTION_COMMON(ThreadException)

    ThreadException( const std::string& msg ) : Exception(msg) {}
    ThreadException() {}
};

class SynchException : public Exception
{
public:
    EXCEPTION_COMMON(SynchException)

    SynchException( const std::string& msg ) : Exception(msg) {}
    SynchException() {}
};

class NotOwnerException : public SynchException
{
public:
    EXCEPTION_COMMON(NotOwnerException)

    NotOwnerException() {}
};

class CantUnlockException : public SynchException
{
public:
    EXCEPTION_COMMON(CantUnlockException)

    CantUnlockException() {}
};

class IllegalPortAccess : public Exception
{
public:
    EXCEPTION_COMMON(IllegalPortAccess)

    IllegalPortAccess(const std::string& componentName) : Exception(componentName + " tried to access a port illegally") {}
};

class InvalidArgumentException : public Exception
{
public:
    EXCEPTION_COMMON(InvalidArgumentException)

    InvalidArgumentException( const std::string& msg ) : Exception(msg) {}
    InvalidArgumentException() {}
};

class NoSuchFunctionException : public Exception
{
public:
    EXCEPTION_COMMON(NoSuchFunctionException)

    NoSuchFunctionException( const std::string& msg ) : Exception(msg) {}
    NoSuchFunctionException() {}
};

class InvalidArgumentCountException : public Exception
{
public:
    EXCEPTION_COMMON(InvalidArgumentCountException)

    InvalidArgumentCountException( const std::string& msg ) : Exception(msg) {}
    InvalidArgumentCountException() {}
};

class InvalidArgumentTypeException : public Exception
{
public:
    EXCEPTION_COMMON(InvalidArgumentTypeException)

    InvalidArgumentTypeException( const std::string& msg ) : Exception(msg) {}
    InvalidArgumentTypeException() {}
};

class IllegalInstructionException : public Exception
{
public:
    EXCEPTION_COMMON(IllegalInstructionException)

    IllegalInstructionException( const std::string& msg ) : Exception(msg) {}
    IllegalInstructionException() {}
};

class IOException : public Exception
{
public:
    EXCEPTION_COMMON(IOException)

    IOException( const std::string& msg ) : Exception(msg) {}
    IOException() {}
};

class FileNotFoundException : public IOException
{
public:
    EXCEPTION_COMMON(FileNotFoundException)

    FileNotFoundException( const std::string& filename ) : IOException("File not found: " + filename) {}
    FileNotFoundException() {}
};

}
#endif

