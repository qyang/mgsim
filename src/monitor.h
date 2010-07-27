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
#ifndef MONITOR_H
# define MONITOR_H

#include "MGSystem.h"
#include "sampling.h"

#include <fstream>
#include <ctime>
#include <pthread.h>

static void* runmonitor(void*);

class Monitor
{
    Simulator::MGSystem&  m_sys;
    std::ofstream*        m_outputfile;
    bool                  m_quiet;
    struct timespec       m_tsdelay;
    
    pthread_t             m_monitorthread;
    pthread_mutex_t       m_runlock;
    bool                  m_running;
    volatile bool         m_enabled;

    BinarySampler*        m_sampler;

    friend void* runmonitor(void*);
    void run();

public:
    Monitor(Simulator::MGSystem& sys, bool enable, const std::string& mdfile, const std::string& outfile, bool quiet);
    ~Monitor();

    void start();
    void stop();
};

#endif
