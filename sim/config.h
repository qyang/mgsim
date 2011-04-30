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
#ifndef CONFIG_H
#define CONFIG_H

#include <algorithm>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <ostream>
#include <cerrno>
#include <cstring>
#include <inttypes.h>
#include "kernel.h"
#include "except.h"

class Config
{
    // a sequence of (pattern, value) pairs 
    typedef std::vector<std::pair<std::string,std::string> > ConfigMap;
    
    typedef std::map<std::string,std::pair<std::string,std::string> > ConfigCache;
    
    ConfigMap         m_data;
    const ConfigMap&  m_overrides;
    ConfigCache       m_cache;

    template<typename T>
    static T convertToNumber(const std::string& name, const std::string& value)
    {
        const char *start = value.c_str();
        char *end;
        T val;

        val = strtoumax(start, &end, 0);
        if (errno == EINVAL)
        {
            val = strtoimax(start, &end, 0);
        }
        if (errno == EINVAL)
        {
            throw Simulator::exceptf<Simulator::SimulationException>("Configuration value for %s is not a number: %s", name.c_str(), start); 
        }
        if (*end != '\0')
        {
            // a prefix is specified, maybe a multiplier?
            if (0 == strcmp(end, "GiB"))
                val *= 1024ULL*1024*1024;
            else if (0 == strcmp(end, "GB"))
                val *= 1000000000ULL;
            else if (0 == strcmp(end, "MiB"))
                val *= 1024*1024;
            else if (0 == strcmp(end, "MB"))
                val *= 1000000;
            else if (0 == strcmp(end, "KiB"))
                val *= 1024;
            else if (0 == strcmp(end, "KB"))
                val *= 1000;
            else
                throw Simulator::exceptf<Simulator::SimulationException>("Configuration value for %s has an invalid suffix: %s", name.c_str(), start); 
        }
        return val;
    }

    bool lookup(const std::string& name, std::string& result, const std::string& def, bool allow_default);

    template <typename T>
    T lookupValue(const std::string& name, const T& def, bool fail_if_not_found) 
    {
        /* general version for numbers. The version for strings and bools is specialized below. */

        std::stringstream ss;
        ss << def;
        std::string val = lookupValue<std::string>(name, ss.str(), fail_if_not_found);
        
        // get the string value back
        return convertToNumber<T>(name, val);
    }

public:

    template <typename T>
    T getValue(const std::string& name, const T& def)
    {
        return lookupValue<T>(name, def, false);
    }

    template <typename T>
    T getValue(const Simulator::Object& obj, const std::string& name, const T& def)
    {
        return lookupValue<T>(obj.GetFQN() + '.' + name, def, false);
    }

    template <typename T>
    T getValue(const std::string& name)
    {
        return lookupValue<T>(name, T(), true);
    }

    template <typename T>
    T getValue(const Simulator::Object& obj, const std::string& name)
    {
        return lookupValue<T>(obj.GetFQN() + '.' + name, T(), true);
    }

    std::vector<std::string> getWordList(const std::string& name); 
    std::vector<std::string> getWordList(const Simulator::Object& obj, const std::string& name);

    void dumpConfiguration(std::ostream& os, const std::string& cf) const;

    Config(const std::string& filename, const ConfigMap& overrides);
};

template <>
std::string Config::lookupValue<std::string>(const std::string& name, const std::string& def, bool fail_if_not_found);

template<>
double Config::convertToNumber<double>(const std::string& name, const std::string& value);

template<>
float Config::convertToNumber<float>(const std::string& name, const std::string& value);

template <>
bool Config::convertToNumber<bool>(const std::string& name, const std::string& value);

#endif
