/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011,2012  The Microgrid Project.

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
#ifndef DELEGATE_H
#define DELEGATE_H

#include <cassert>

namespace Simulator
{

class Object;

enum Result
{
    FAILED,
    DELAYED,
    SUCCESS
};

class delegate
{
    void     *m_object;
    Result  (*m_stub)(void*);
    Object* (*m_get)(void*);
    
    template <typename T, Result (T::*TMethod)()>
    static Result method_stub(void* object)
    {
        T* p = static_cast<T*>(object);
        return (p->*TMethod)();
    }

    template <typename T>
    static Object* get_stub(void* object)
    {
        return static_cast<T*>(object);
    }
    
    delegate() {}
public:
    template <typename T, Result (T::*TMethod)()>
    static delegate create(T& object)
    {
        delegate d;
        d.m_object = &object;
        d.m_stub   = &method_stub<T,TMethod>;
        d.m_get    = &get_stub<T>;
        return d;
    }
    
    Result operator()() const
    {
        return (*m_stub)(m_object);
    }
    
    Object* GetObject() const { return m_get(m_object); }   
};    

}

#endif
