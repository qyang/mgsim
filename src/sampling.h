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
#ifndef SAMPLING_H
#define SAMPLING_H

#include "MGSystem.h"

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <cassert>

enum SampleVariableDataType {
    SV_INTEGER,
    SV_FLOAT
};

enum SampleVariableCategory {
    SVC_LEVEL,    // current level: cycle counter, #threads, etc 
    SVC_STATE,    // state variable
    SVC_WATERMARK,  // mins and maxs, evolves monotonously
    SVC_CUMULATIVE, // integral of level over time
};

template<typename T> struct _sv_detect_type { static const SampleVariableDataType type = SV_INTEGER; };
template<> struct _sv_detect_type<float> { static const SampleVariableDataType type = SV_FLOAT; };
template<> struct _sv_detect_type<double> { static const SampleVariableDataType type = SV_FLOAT; };

template<typename T>
void RegisterSampleVariable(T& var, const std::string& name, SampleVariableCategory cat, T max = (T)0)
{
    extern void _RegisterSampleVariable(void*, size_t, const std::string&, SampleVariableDataType, SampleVariableCategory, void*);
    _RegisterSampleVariable(&var, sizeof(T), name, _sv_detect_type<T>::type, cat, &max);
}

#define RegisterSampleVariableInObject(var, cat, ...)                       \
    RegisterSampleVariable(var, GetFQN() + '.' + (#var + 2) , cat, ##__VA_ARGS__)

#define RegisterSampleVariableInObjectWithName(var, name, cat, ...)      \
    RegisterSampleVariable(var, GetFQN() + '.' + name, cat, ##__VA_ARGS__)

void ListSampleVariables(std::ostream& os, const std::string &pat = "*");
void ShowSampleVariables(std::ostream& os, const std::string &pat = "*");

class BinarySampler
{
    typedef std::vector<std::pair<const char*, size_t> > vars_t;

    size_t   m_datasize;
    vars_t   m_vars;

public:

    BinarySampler(std::ostream& os, const Simulator::MGSystem& sys, 
                  const std::vector<std::string>& pats);

    size_t GetBufferSize() const { return m_datasize; }
    void   SampleToBuffer(char *buf) const
    {
        for (vars_t::const_iterator i = m_vars.begin(); i != m_vars.end(); ++i)
            for (size_t j = 0; j < i->second; ++j)
                *buf++ = i->first[j];
    }
};

#endif
