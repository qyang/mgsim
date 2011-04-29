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
#include "kernel.h"

class Config
{
    // a sequence of (pattern, value) pairs 
    typedef std::vector<std::pair<std::string,std::string> > ConfigMap;
    
    // a cache of (key, (value, origin pattern)) pairs
    typedef std::map<std::string,std::pair<std::string,std::string> > ConfigCache;
    
    ConfigMap         m_data;
    const ConfigMap&  m_overrides;
    ConfigCache       m_cache;

public:

    template <typename T>
    T getValue(const std::string& name, const T& def) 
    {
        T val;
        std::stringstream defv;
        defv << def;
        std::stringstream stream;
        stream << getValue<std::string>(name, defv.str());
        stream >> val;
        return (!stream.fail() && stream.eof()) ? val : def;
    }

    template <typename T>
    T getValue(const Simulator::Object& obj, const std::string& name, const T& def)
    {
        return getValue(obj.GetFQN() + '.' + name, def);
    }

    template <typename T>
    std::vector<T> getValueList(const std::string& name) 
    {
        std::vector<T> vals;
        std::string str = getValue<std::string>(name,"");
        std::istringstream stream(str);
        std::string token = "";
        while (getline(stream, token, ','))
        {
            std::stringstream ss;
            T val;
            ss << token;
            ss >> val;
            if (ss.fail()) break;
            vals.push_back(val);
        }
        return vals;
    }

    template <typename T>
    std::vector<T> getValueList(const Simulator::Object& obj, const std::string& name)
    {
        return getValueList<T>(obj.GetFQN() + '.' + name);
    }

    template <typename T>
    T getSize(const std::string& name, const T& def)
    {
        T val;
        std::stringstream defv;
        defv << def;
        std::string strmult;
        std::stringstream stream;
        stream << getValue<std::string>(name, defv.str());
        stream >> val;
        stream >> strmult;
        if (!strmult.empty())
        {
            if (strmult.size() == 2)
            {
                switch (toupper(strmult[0]))
                {
                    default: return def;
                    case 'G': val = val * 1024;
                    case 'M': val = val * 1024;
                    case 'K': val = val * 1024;
                }                
                strmult = strmult.substr(1);
            }

            if (strmult.size() != 1 || toupper(strmult[0]) != 'B')
            {
                return def;
            }
        }
        return (!stream.fail() && stream.eof()) ? val : def;
    }

    template <typename T>
    T getSize(const Simulator::Object& obj, const std::string& name, const T& def)
    {
        return getValueSize(obj.GetFQN() + '.' + name, def);
    }

    void dumpConfiguration(std::ostream& os, const std::string& cf) const;

    Config(const std::string& filename, const ConfigMap& overrides);
};

template <>
bool Config::getValue<bool>(const std::string& name, const bool& def);

template <>
std::string Config::getValue<std::string>(const std::string& name, const std::string& def);

#endif
