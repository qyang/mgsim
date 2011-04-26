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
#ifndef INSPECT_H
#define INSPECT_H

#include <iostream>
#include <vector>
#include <string>

namespace Inspect
{
    enum InterfaceTypes
    {
        None = 0,
        Help = 1,
        Info = 2,
        Read = 4,
        Trace = 8,
        Line = 16,
        LastType = Line
    };

    template<unsigned V>
        class Interface_;

    template<unsigned V, unsigned Choose>
        class Parent_ : public Parent_<V, (Choose >> 1)>, public virtual Interface_<V & Choose>
    { };    
    
    class ListCommands
    {
    public:
        void ListSupportedCommands(std::ostream&) const;
        virtual ~ListCommands() {};
    };

    template<unsigned V>
        class Parent_<V, 0> : public virtual ListCommands
    { };

    template<unsigned V>
        class Interface :
        public Parent_<V|Help, LastType>
    {
    public:
        virtual ~Interface() {}
    };
    
    template<>
        class Interface_<None>
    {
    public:
        virtual ~Interface_() {}
    };
    
    template<>
        class Interface_<Help>
    {
    public:
        virtual void Cmd_Help(std::ostream&, const std::vector<std::string>&) const = 0;
        virtual ~Interface_() {}
        void DoCommand(std::ostream& s, const std::vector<std::string>& v) const { return Cmd_Help(s, v); }
    };

    template<>
        class Interface_<Info>
    {
    public:
        virtual void Cmd_Info(std::ostream&, const std::vector<std::string>&) const = 0;
        virtual ~Interface_() {}
        void DoCommand(std::ostream& s, const std::vector<std::string>& v) const { return Cmd_Info(s, v); }
    };

    template<>
        class Interface_<Read>
    {
    public:
        virtual void Cmd_Read(std::ostream&, const std::vector<std::string>&) const = 0;
        virtual ~Interface_() {}
        void DoCommand(std::ostream& s, const std::vector<std::string>& v) const { return Cmd_Read(s, v); }
    };

    template<>
        class Interface_<Trace>
    {
    public:
        virtual void Cmd_Trace(std::ostream&, const std::vector<std::string>&) = 0;
        virtual ~Interface_() {}
        void DoCommand(std::ostream& s, const std::vector<std::string>& v) { return Cmd_Trace(s, v); }
    };

    template<>
        class Interface_<Line>
    {
    public:
        virtual void Cmd_Line(std::ostream&, const std::vector<std::string>&) const = 0;
        virtual ~Interface_() {}
        void DoCommand(std::ostream& s, const std::vector<std::string>& v) const { return Cmd_Line(s, v); }
    };
}

#endif
