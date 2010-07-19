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
#include "monitor.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <signal.h>

#define pthread(Function, ...) do { if (pthread_ ## Function(__VA_ARGS__)) perror("pthread_" #Function); } while(0)

static 
void* runmonitor(void *arg)
{
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGQUIT);
    sigaddset(&sigset, SIGHUP);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, 0);

    Monitor *m = (Monitor*) arg;
    m->run();
    return 0;
}

Monitor::Monitor(Simulator::MGSystem& sys, const Config& config, const std::string& outfile, bool quiet)
    : m_sys(sys), 
      m_config(config),
      m_outputfile(0),
      m_quiet(quiet),
      m_running(false),
      m_enabled(true)
{
    if (outfile.size() == 0) 
    {
        if (!m_quiet)
            std::clog << "# monitoring disabled." << std::endl;
        return ;
    }

    m_outputfile = new std::ofstream(outfile.c_str());
    if (!m_outputfile->good()) 
    {
        std::clog << "# warning: cannot write to file " << outfile << ". Monitoring disabled." << std::endl;
        delete m_outputfile;
        m_outputfile = 0;
        return ;
    }

    float msd = m_config.getInteger<float>("MonitorSampleDelay", 0.001);
    msd = fabs(msd);
    m_tsdelay.tv_sec = msd;
    m_tsdelay.tv_nsec = (msd - (float)m_tsdelay.tv_sec) * 1000000000.;
   
    if (!m_quiet)
        std::clog << "# monitoring enabled, sampling every "
                  << m_tsdelay.tv_sec << '.'
                  << std::setfill('0') << std::setw(9) << m_tsdelay.tv_nsec 
                  << "s to file " << outfile << std::endl;

    pthread(mutex_init, &m_runlock, 0);
    pthread(mutex_lock, &m_runlock);

    pthread(create, &m_monitorthread, 0, runmonitor, this);
}

Monitor::~Monitor()
{
    if (m_outputfile) 
    {
        if (!m_quiet)
            std::clog << "# shutting down monitoring..." << std::endl;

        m_enabled = false;
        pthread(mutex_unlock, &m_runlock);
        pthread(join, m_monitorthread, 0);
        pthread(mutex_destroy, &m_runlock);

        m_outputfile->flush();
        delete m_outputfile;
        if (!m_quiet)
            std::clog << "# monitoring ended." << std::endl;
    }
}

void Monitor::start()
{
    if (m_outputfile) {
        if (!m_quiet)
            std::clog << "# starting monitor..." << std::endl;
        m_running = true;
        pthread(mutex_unlock, &m_runlock);
    }
}

void Monitor::stop()
{
    if (m_running) {
        if (!m_quiet)
            std::clog << "# stopping monitor..." << std::endl;
        pthread(mutex_lock, &m_runlock);
        m_running = false;
    }
}

void Monitor::run()
{
    if (!m_quiet)
        std::clog << "# monitor thread started." << std::endl;

    while (m_enabled) 
    {
        nanosleep(&m_tsdelay, 0);
        pthread(mutex_lock, &m_runlock);
        
        pthread(mutex_unlock, &m_runlock);
    }
}
