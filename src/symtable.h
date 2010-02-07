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
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <iostream>

#include "simtypes.h"

class SymbolTable 
{
public:
    typedef std::pair<size_t, std::string> sym_t;
    typedef std::pair<Simulator::MemAddr, sym_t> entry_t;
    
    // default constructor & copy constructor are suitable.

    void Read(std::istream& i);
    void AddSymbol(Simulator::MemAddr addr, const std::string& name, size_t sz = 0);

    void Write(std::ostream& o, const std::string& pat = "*") const;

    const std::string& operator[](Simulator::MemAddr addr);
    const std::string operator[](Simulator::MemAddr addr) const;
    
protected:     
    const std::string Resolve(Simulator::MemAddr addr) const;

    typedef std::vector<entry_t> table_t;
    table_t m_entries;
    typedef std::map<Simulator::MemAddr, std::string> cache_t;
    cache_t m_cache;
};


#endif
