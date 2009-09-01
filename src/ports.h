/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009  The Microgrid Project.

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
#ifndef PORTS_H
#define PORTS_H

#include "kernel.h"
#include <cassert>
#include <map>
#include <set>
#include <limits>

namespace Simulator
{

template <typename I> class Structure;
template <typename I> class ArbitratedWritePort;
class ArbitratedReadPort;

//
// ArbitrationSource
//
// This identifies any 'source' for arbitrated requests.
// It consists of a component ID and state index.
//
struct ArbitrationSource : public std::pair<IComponent*, int>
{
    ArbitrationSource() {}
    
    ArbitrationSource(IComponent* component, int state) 
        : std::pair<IComponent*, int>(component, state) {
    }
    
    explicit ArbitrationSource(const Kernel::ProcessInfo* p)
        : std::pair<IComponent*, int>(p->info->component, p - &p->info->processes[0]) {
    }
};

//
// IllegalPortAccess
//
// Exception thrown when a component accesses a port or service
// that it has not been allowed to di.
//
class IllegalPortAccess : public SimulationException
{
    static std::string ConstructString(const Object& object, const std::string& name, const ArbitrationSource& src);
public:
    IllegalPortAccess(const Object& object, const std::string& name, const ArbitrationSource& src)
        : SimulationException(ConstructString(object, name, src)) {}
};

//
// ArbitratedPort
//
class ArbitratedPort
{
    typedef std::vector<ArbitrationSource>  RequestList;
    typedef std::map<ArbitrationSource,int> PriorityMap;

public:
	uint64_t GetBusyCycles() const {
		return m_busyCycles;
	}

    void AddSource(const ArbitrationSource& source) {
        m_priorities.insert(PriorityMap::value_type(source, m_priorities.size()));
    }

    void Arbitrate();
    
protected:
    bool HasAcquired(const ArbitrationSource& source) const {
        return m_source == source;
    }
    
    const ArbitrationSource& GetArbitratedSource() const {
        return m_source;
    }

#ifndef NDEBUG
    void Verify(const ArbitrationSource& source) const {
        if (m_priorities.find(source) == m_priorities.end()) {
            throw IllegalPortAccess(m_object, m_name, source);
        }
    }
#else
    void Verify(const ArbitrationSource& /* source */) const {}
#endif

    void AddRequest(const ArbitrationSource& source);

    ArbitratedPort(const Object& object, const std::string& name) : m_busyCycles(0), m_object(object), m_name(name) {}
    virtual ~ArbitratedPort() {}

private:
    PriorityMap       m_priorities;
    RequestList       m_requests;
    ArbitrationSource m_source;
	uint64_t          m_busyCycles;
	const Object&     m_object;
    std::string       m_name;
};

//
// WritePort
//
// Write ports have an index, because if two ports write to the same index
// in a structure, only one can proceed. So the index needs to be captured
// for arbitration.
//
template <typename I>
class WritePort
{
    bool    m_valid;    ///< Is there a request?
    bool    m_chosen;   ///< Have we been chosen?
    I       m_index;    ///< Index of the request
    
protected:
    void SetIndex(const I& index)
    {
        m_index  = index;
        m_valid  = true;
        m_chosen = false;
    }
    
    WritePort()
        : m_valid(false), m_chosen(false)
    {
    }

public:
    bool     IsChosen() const { return m_chosen; }
    const I* GetIndex() const { return (m_valid) ? &m_index : NULL; }
    
    void Notify(bool chosen) {
        m_chosen = chosen;
        m_valid  = false;
    }
};

//
// Structure
//

/// Base class for all structures
class IStructure : public Object, private Arbitrator
{
    typedef std::set<ArbitratedReadPort*> ReadPortList;

    ReadPortList m_readPorts;

protected:
    void ArbitrateReadPorts();

public:
    void RequestArbitration()
    {
        Arbitrator::RequestArbitration();
    }
    
    /**
     * @brief Constructs the structure
     * @param parent parent object.
     * @param kernel the kernel which will manage this structure.
     * @param name name of the object.
     */
    IStructure(Object* parent, Kernel& kernel, const std::string& name);

    void RegisterReadPort(ArbitratedReadPort& port);
    void UnregisterReadPort(ArbitratedReadPort& port);
};

template <typename I>
class Structure : public IStructure
{
    typedef std::map<WritePort<I>*, int>      PriorityMap;
    typedef std::set<ArbitratedWritePort<I>*> ArbitratedWritePortList;
    typedef std::set<WritePort<I>*>           WritePortList;

    void OnArbitrate()
    {
        typedef std::vector<WritePort<I>*> WritePortMap;
        typedef std::map<I,WritePortMap>   RequestPortMap;
        
        // Tell each port to arbitrate
        ArbitrateReadPorts();
        for (typename ArbitratedWritePortList::iterator i = m_arbitratedWritePorts.begin(); i != m_arbitratedWritePorts.end(); ++i)
        {
            (*i)->Arbitrate();
        }

        // Get the final requests from all ports
        RequestPortMap requests;
        for (typename WritePortList::iterator i = m_writePorts.begin(); i != m_writePorts.end(); ++i)
        {
            const I* index = (*i)->GetIndex();
            if (index != NULL)
            {
                requests[*index].push_back(*i);
            }
        }

        // Arbitrate between the ports for each request
        for (typename RequestPortMap::iterator i = requests.begin(); i != requests.end(); ++i)
        {
            WritePort<I>* port = NULL;
            int highest = std::numeric_limits<int>::max();

            for (typename WritePortMap::iterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                typename PriorityMap::const_iterator priority = m_priorities.find(*j);
                if (priority != m_priorities.end() && priority->second < highest)
                {
                    highest   = priority->second;
                    port      = *j;
                }
            }

            for (typename WritePortMap::iterator j = i->second.begin(); j != i->second.end(); ++j)
            {
                (*j)->Notify( port == *j );
            }
        }
    }

    WritePortList           m_writePorts;
    ArbitratedWritePortList m_arbitratedWritePorts;
    PriorityMap             m_priorities;

public:
    Structure(Object* parent, Kernel& kernel, const std::string& name) : IStructure(parent, kernel, name) {}

    void AddPort(WritePort<I>& port)
    {
        m_priorities.insert(typename PriorityMap::value_type(&port, m_priorities.size()));
    }

    void RegisterWritePort(WritePort<I>& port)                        { m_writePorts.insert(&port); }
    void UnregisterWritePort(WritePort<I>& port)                      { m_writePorts.erase(&port);  }
    void RegisterArbitratedWritePort(ArbitratedWritePort<I>& port)    { RegisterWritePort(port);   m_arbitratedWritePorts.insert(&port); }
    void UnregisterArbitratedWritePort(ArbitratedWritePort<I>& port)  { UnregisterWritePort(port); m_arbitratedWritePorts.erase (&port); }
};

//
// ArbitratedReadPort
//
class ArbitratedReadPort : public ArbitratedPort
{
    IStructure& m_structure;
public:
    ArbitratedReadPort(IStructure& structure, const std::string& name)
        : ArbitratedPort(structure, name), m_structure(structure)
    {
        m_structure.RegisterReadPort(*this);
    }
    
    ~ArbitratedReadPort() {
        m_structure.UnregisterReadPort(*this);
    }

    bool Read()
    {
        const ArbitrationSource source(m_structure.GetKernel()->GetActiveProcess());
        Verify(source);
        if (m_structure.GetKernel()->GetCyclePhase() == PHASE_ACQUIRE)
        {
            AddRequest(source);
            m_structure.RequestArbitration();
        }
        else if (!HasAcquired(source))
        {
            return false;
        }
        return true;
    }
};

//
// ArbitratedWritePort
//
template <typename I>
class ArbitratedWritePort : public ArbitratedPort, public WritePort<I>
{
    typedef std::map<ArbitrationSource, I> IndexMap;
   
    Structure<I>& m_structure;
    IndexMap      m_indices;

    void AddRequest(const ArbitrationSource& source, const I& index)
    {
        ArbitratedPort::AddRequest(source);
        m_indices[source] = index;
    }
    
public:
    void Arbitrate()
    {
        ArbitratedPort::Arbitrate();
        const ArbitrationSource& source = GetArbitratedSource();
        if (source != ArbitrationSource())
        {
            // A source was selected; make its index active for
            // write port arbitration
            typename IndexMap::const_iterator p = m_indices.find(source);
            assert(p != m_indices.end());
            this->SetIndex(p->second);
        }
    }

    ArbitratedWritePort(Structure<I>& structure, const std::string& name)
        : ArbitratedPort(structure, name), m_structure(structure)
    {
        m_structure.RegisterArbitratedWritePort(*this);
    }
    
    ~ArbitratedWritePort() {
        m_structure.UnregisterArbitratedWritePort(*this);
    }

    bool Write(const I& index)
    {
        const ArbitrationSource source(m_structure.GetKernel()->GetActiveProcess());
        Verify(source);
        if (m_structure.GetKernel()->GetCyclePhase() == PHASE_ACQUIRE)
        {
            AddRequest(source, index);
            m_structure.RequestArbitration();
        }
        else if (!this->IsChosen() || !HasAcquired(source))
        {
            return false;
        }
        return true;
    }
};

//
// DedicatedPort
//
class DedicatedPort
{
public:
    DedicatedPort(const Object& object, const std::string& name) : m_object(object), m_name(name) {}
    virtual ~DedicatedPort() {}

    void SetSource(ArbitrationSource source) {
        m_source = source;
    }
protected:
#ifndef NDEBUG
    void Verify(ArbitrationSource source) {
        if (m_source != source) {
            throw IllegalPortAccess(m_object, m_name, source);
        }
#else
    void Verify(ArbitrationSource /* source */) {
#endif
    }
private:
    ArbitrationSource m_source;
    const Object&     m_object;
    std::string       m_name;
};

//
// DedicatedReadPort
//
class DedicatedReadPort : public DedicatedPort
{
    IStructure& m_structure;
public:
    DedicatedReadPort(IStructure& structure, const std::string& name)
        : DedicatedPort(structure, name), m_structure(structure) {}
        
    bool Read() {
        const ArbitrationSource source(m_structure.GetKernel()->GetActiveProcess());
        // Dedicated Read ports always succeed -- they don't require arbitration
        Verify(source);
        return true;
    }
};

//
// DedicatedWritePort
//
template <typename I>
class DedicatedWritePort : public DedicatedPort, public WritePort<I>
{
    Structure<I>& m_structure;
public:
    DedicatedWritePort(Structure<I>& structure, const std::string& name)
        : DedicatedPort(structure, name), m_structure(structure)
    {
        m_structure.RegisterWritePort(*this);
    }
    
    ~DedicatedWritePort() {
        m_structure.UnregisterWritePort(*this);
    }

    bool Write(const I& index) {
        const ArbitrationSource source(m_structure.GetKernel()->GetActiveProcess());
        DedicatedPort::Verify(source);
        if (m_structure.GetKernel()->GetCyclePhase() == PHASE_ACQUIRE) {
            this->SetIndex(index);
            m_structure.RequestArbitration();
        } else if (!this->IsChosen()) {
            return false;
        }
        return true;
    }
};

//
// ArbitratedService
//
// An arbitrated service is like an arbitrated port, only there's no
// associated service. It's purpose is to arbitrate access to a single
// feature of a component.
//
class ArbitratedService : public ArbitratedPort, public Arbitrator
{
    void OnArbitrate();
    
public:
    bool Invoke()
    {
        const ArbitrationSource source(m_kernel.GetActiveProcess());
        Verify(source);
        if (m_kernel.GetCyclePhase() == PHASE_ACQUIRE) {
            this->AddRequest(source);
            RequestArbitration();
        } else if (!this->HasAcquired(source)) {
            return false;
        }
        return true;
    }
    
    ArbitratedService(const Object& object, const std::string& name)
        : ArbitratedPort(object, name), Arbitrator(*object.GetKernel())
    {
    }
    
    ~ArbitratedService() {
    }
};

}
#endif

