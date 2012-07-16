#include "Processor.h"
#include "sim/config.h"
#include <cstring>

namespace Simulator
{
    Processor::IODirectCacheAccess::IODirectCacheAccess(const std::string& name, Object& parent, Clock& clock, Processor& proc, IMemory& memory, IOBusInterface& busif, Config& config)
        : Object(name, parent, clock),
          m_cpu(proc),
          m_memory(memory),
          m_busif(busif),
          m_lineSize(config.getValue<MemSize>("CacheLineSize")),
          m_requests("b_requests", *this, clock, config.getValue<BufferSize>(*this, "RequestQueueSize")),
          m_responses("b_responses", *this, clock, config.getValue<BufferSize>(*this, "ResponseQueueSize")),
          m_has_outstanding_request(false),
          m_flushing(false),
          m_pending_writes(0),
          p_MemoryOutgoing(*this, "send-memory-requests", delegate::create<IODirectCacheAccess, &Processor::IODirectCacheAccess::DoMemoryOutgoing>(*this)),
          p_BusOutgoing   (*this, "send-bus-responses", delegate::create<IODirectCacheAccess, &Processor::IODirectCacheAccess::DoBusOutgoing>(*this)),
          p_service(*this, clock, "p_service")
    {
        p_service.AddProcess(p_BusOutgoing);
        p_service.AddProcess(p_MemoryOutgoing);
        m_responses.Sensitive(p_BusOutgoing);
        m_requests.Sensitive(p_MemoryOutgoing);
        
        StorageTraceSet traces;
        m_mcid = m_memory.RegisterClient(*this, p_MemoryOutgoing, traces, m_responses, true);
        
        p_MemoryOutgoing.SetStorageTraces(opt(traces ^ m_responses));
        p_BusOutgoing.SetStorageTraces(opt(m_busif.m_outgoing_reqs));
    }
    
    Processor::IODirectCacheAccess::~IODirectCacheAccess()
    {
        m_memory.UnregisterClient(m_mcid);
    }
    
    bool Processor::IODirectCacheAccess::QueueRequest(const Request& req)
    {
        size_t offset = (size_t)(req.address % m_lineSize);
        if (offset + req.data.size > m_lineSize)
        {
            throw exceptf<InvalidArgumentException>(*this, "DCA request for %#016llx/%u (dev %u, type %d) crosses over cache line boundary", 
                                                    (unsigned long long)req.address, (unsigned)req.data.size, (unsigned)req.client, (int)req.type);
        }

        if (req.type != FLUSH && !m_cpu.CheckPermissions(req.address, req.data.size, (req.type == WRITE) ? IMemory::PERM_DCA_WRITE : IMemory::PERM_DCA_READ))
        {
            throw exceptf<SecurityException>(*this, "Invalid access in DCA request for %#016llx/%u (dev %u, type %d)",
                                             (unsigned long long)req.address, (unsigned)req.data.size, (unsigned)req.client, (int)req.type);
        }

        if (!m_requests.Push(req))
        {
            DeadlockWrite("Unable to queue DCA request (%#016llx/%u, dev %u, type %d",
                          (unsigned long long)req.address, (unsigned)req.data.size, (unsigned)req.client, (int)req.type);
            return false;
        }

        DebugIOWrite("Queued DCA request (%#016llx/%u, dev %u, type %d)",
                     (unsigned long long)req.address, (unsigned)req.data.size, (unsigned)req.client, (int)req.type);

        return true;
    }

    bool Processor::IODirectCacheAccess::OnMemoryWriteCompleted(TID tid)
    {
        if (tid == INVALID_TID) // otherwise for D-Cache
        {
            Response res;
            res.address = 0;
            res.data.size = 0;
            if (!m_responses.Push(res))
            {
                DeadlockWrite("Unable to push memory write response");
                return false;
            }
        }
        return true;
    }

    bool Processor::IODirectCacheAccess::OnMemoryReadCompleted(MemAddr addr, const MemData& data)
    {
        assert(addr % m_lineSize == 0);
        assert(data.size == m_lineSize);

        Response res;
        res.address = addr;
        res.data = data;
        if (!m_responses.Push(res))
        {
            DeadlockWrite("Unable to push memory read response (%#016llx, %u)",
                          (unsigned long long)addr, (unsigned)data.size);
            
            return false;
        }
        return true;
    }

    Result Processor::IODirectCacheAccess::DoBusOutgoing()
    {
        const Response& res = m_responses.Front();


        if (!p_service.Invoke())
        {
            DeadlockWrite("Unable to acquire port for DCA read response (%#016llx, %u)",
                          (unsigned long long)res.address, (unsigned)res.data.size);
            
            return FAILED;
        }

        if (res.address == 0 && res.data.size == 0)
        {
            // write response
            assert(m_pending_writes > 0);
            
            if (m_pending_writes == 1 && m_flushing == true)
            { 
                COMMIT { 
                    --m_pending_writes;
                    m_flushing = false; 
                }
                // the flush response will be sent below.
            }
            else
            {
                COMMIT { 
                    --m_pending_writes; 
                }
                // one or more outstanding write left, no flush response needed
                m_responses.Pop();
                return SUCCESS;
            }
        }

        if (m_has_outstanding_request 
            && res.address <= m_outstanding_address
            && res.address + res.data.size >= m_outstanding_address + m_outstanding_size)
        {
            IOBusInterface::IORequest req;
            req.device = m_outstanding_client;
            req.type = IOBusInterface::REQ_READRESPONSE;
            req.address = m_outstanding_address;
            req.data.size = m_outstanding_size;
            memcpy(req.data.data, res.data.data + (m_outstanding_address - res.address), m_outstanding_size);
            
            if (!m_busif.SendRequest(req))
            {
                DeadlockWrite("Unable to send DCA read response to client %u for %#016llx/%u",
                              (unsigned)req.device, (unsigned long long)req.address, (unsigned)req.data.size);
                return FAILED;
            }

            DebugIOWrite("Sent DCA read response to client %u for %#016llx/%u",
                         (unsigned)req.device, (unsigned long long)req.address, (unsigned)req.data.size);
            
            COMMIT {
                m_has_outstanding_request = false;
            }
        }

        m_responses.Pop();

        return SUCCESS;
    }

    Result Processor::IODirectCacheAccess::DoMemoryOutgoing()
    {
        const Request& req = m_requests.Front();

        switch(req.type)
        {
        case FLUSH:
        {
            if (!p_service.Invoke())
            {
                DeadlockWrite("Unable to acquire port for DCA flush");                
                return FAILED;
            }

            if (m_has_outstanding_request)
            {
                // some request is already queued, so we just stall
                DeadlockWrite("Will not send additional DCA flush request from client %u, already waiting for request for dev %u",
                              (unsigned)req.client, (unsigned)m_outstanding_client);
                return FAILED;
            }

            if (m_pending_writes == 0)
            {
                // no outstanding write, fake one and then 
                // acknowledge immediately

                COMMIT { ++m_pending_writes; }

                Response res;
                res.address = 0;
                res.data.size = 0;
                if (!m_responses.Push(res))
                {
                    DeadlockWrite("Unable to push DCA flush response");
                    return FAILED;
                }
            }

            COMMIT {
                m_flushing = true;
                m_has_outstanding_request = true;
                m_outstanding_client = req.client;
                m_outstanding_address = 0;
                m_outstanding_size = 0;
            }
            
            break;
        }
        case READ:
        {
            // this is a read request coming from the bus.
            
            if (req.data.size > m_lineSize || req.data.size > MAX_MEMORY_OPERATION_SIZE)
            {
                throw InvalidArgumentException("Read size is too big");
            }
            
            if (req.address / m_lineSize != (req.address + req.data.size - 1) / m_lineSize)
            {
                throw InvalidArgumentException("Read request straddles cache-line boundary");
            }

            
            if (!p_service.Invoke())
            {
                DeadlockWrite("Unable to acquire port for DCA read (%#016llx, %u)",
                              (unsigned long long)req.address, (unsigned)req.data.size);
                
                return FAILED;
            }

            if (m_has_outstanding_request)
            {
                // some request is already queued, so we just stall
                DeadlockWrite("Will not send additional DCA read request from client %u for %#016llx/%u, already waiting for %#016llx/%u, dev %u",
                              (unsigned)req.client, (unsigned long long)req.address, (unsigned)req.data.size, 
                              (unsigned long long)m_outstanding_address, (unsigned)m_outstanding_size, (unsigned)m_outstanding_client);
                return FAILED;
            }
            
            // send the request to the memory
            MemAddr line_address  = req.address & -m_lineSize;
            if (!m_memory.Read(m_mcid, line_address, m_lineSize))
            {
                DeadlockWrite("Unable to send DCA read from %#016llx/%u, dev %u to memory", (unsigned long long)req.address, (unsigned)req.data.size, (unsigned)req.client);
                return FAILED;
            }

            COMMIT {
                m_has_outstanding_request = true;
                m_outstanding_client = req.client;
                m_outstanding_address = req.address;
                m_outstanding_size = req.data.size;
            }

            break;
        }
        case WRITE:
        {
            // write operation

            if (req.data.size > m_lineSize || req.data.size > MAX_MEMORY_OPERATION_SIZE)
            {
                throw InvalidArgumentException("Write size is too big");
            }
            
            if (req.address / m_lineSize != (req.address + req.data.size - 1) / m_lineSize)
            {
                throw InvalidArgumentException("Write request straddles cache-line boundary");
            }

            if (!p_service.Invoke())
            {
                DeadlockWrite("Unable to acquire port for DCA write (%#016llx, %u)",
                              (unsigned long long)req.address, (unsigned)req.data.size);
                
                return FAILED;
            }

            char data[m_lineSize];
            bool mask[m_lineSize];
            MemAddr line_addr = req.address & -m_lineSize;
            size_t offset = req.address & (m_lineSize - 1);
            
            std::copy(req.data.data, req.data.data + req.data.size, data + offset);
            std::fill(mask, mask + offset, false);
            std::fill(mask + offset, mask + offset + req.data.size, true);
            std::fill(mask + offset + req.data.size, mask + m_lineSize, false);
            
            if (!m_memory.Write(m_mcid, line_addr, data, m_lineSize, INVALID_LFID, mask, false))
            {
                DeadlockWrite("Unable to send DCA write to %#016llx/%u to memory", (unsigned long long)req.address, (unsigned)req.data.size);
                return FAILED;
            }

            COMMIT { ++m_pending_writes; }

            break;
        }
        }

        m_requests.Pop();
        return SUCCESS;

    }


}
