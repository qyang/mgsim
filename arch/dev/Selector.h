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
#ifndef SELECTOR_H
#define SELECTOR_H

#include "sim/delegate.h"
#include "sim/kernel.h"
#include "sim/storage.h"
#include "sim/inspect.h"

class Config;

namespace Simulator
{

    class ISelectorClient;

    class Selector : public Object, public Inspect::Interface<Inspect::Info>
    {
        static Selector*     m_singleton;

        SingleFlag m_doCheckStreams;

    public:

        enum StreamState
        {
            READABLE = 1,
            WRITABLE = 2,
        };

        Selector(const std::string& name, Object& parent, Clock& clock, Config& config);
        ~Selector();

        Process p_checkStreams;

        Result DoCheckStreams();

        bool RegisterStream(int fd, ISelectorClient& callback);
        bool UnregisterStream(int fd);

        static Selector& GetSelector();

        // Admin
        void Enable();
        void Disable();

        /* debug */
        void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const;
    };

    class ISelectorClient
    {
    public:
        virtual bool OnStreamReady(int fd, Selector::StreamState state) = 0;
        virtual std::string GetSelectorClientName() const = 0;
        virtual ~ISelectorClient() {}
    };


}

#endif
