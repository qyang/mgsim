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
#ifndef BREAKPOINTS_H
#define BREAKPOINTS_H

#include <map>
#include <string>

#include "sim/kernel.h"
#include "arch/symtable.h"
#include "sim/except.h"
#include <iostream>

class BreakPoints
{
public:
    enum BreakPointType { EXEC = 1, READ = 2, WRITE = 4, TRACEONLY = 8 };

private:
    struct BreakPointInfo {
        bool              enabled;
        unsigned          id;
        int               type;
    };

    typedef std::map<Simulator::MemAddr, BreakPointInfo> breakpoints_t;

    struct ActiveBreak {
        Simulator::MemAddr addr;
        Simulator::Object  *obj;
        int                type;

        ActiveBreak(Simulator::MemAddr addr_, Simulator::Object& obj_, int type_)
        : addr(addr_), obj(&obj_), type(type_) {}

        // For std::set
        bool operator<(const ActiveBreak& other) const
        { 
            return (addr < other.addr) ||
                (addr == other.addr && (obj < other.obj)) ||
                (obj == other.obj && type < other.type); 
        }
    };

    typedef std::set<ActiveBreak> active_breaks_t;


    unsigned           m_counter;
    bool               m_enabled;
    breakpoints_t      m_breakpoints;
    active_breaks_t    m_activebreaks;
    Simulator::Kernel& m_kernel;

    void CheckMore(int type, Simulator::MemAddr addr, Simulator::Object& obj);
    void CheckEnabled(void);

    static std::string GetModeName(int);
public:
    BreakPoints(Simulator::Kernel& kernel) 
        : m_counter(0), m_enabled(false), m_kernel(kernel) {}

    void EnableCheck(void) { m_enabled = true; }
    void DisableCheck(void) { m_enabled = false; }

    void EnableBreakPoint(unsigned id);
    void DisableBreakPoint(unsigned id);
    void DeleteBreakPoint(unsigned id);

    void AddBreakPoint(Simulator::MemAddr addr, int type = EXEC);
    void AddBreakPoint(const std::string& sym, int offset, int type = EXEC);

    void ClearAllBreakPoints(void);
    void ListBreakPoints(std::ostream& out) const;

    // Call Resume() once after some breakpoints have been
    // encountered and before execution should resume.
    void Resume(void);

    void ReportBreaks(std::ostream& out) const;

    bool NewBreaksDetected(void) const { return !m_activebreaks.empty(); }

    void Check(int type, Simulator::MemAddr addr, Simulator::Object& obj) 
    {
        if (m_enabled)
            CheckMore(type, addr, obj);
    }
};

#endif
