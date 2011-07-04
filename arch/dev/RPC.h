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
#ifndef RPC_H
#define RPC_H

#include "kernel.h"
#include "storage.h"
#include "IOBus.h"

namespace Simulator 
{
    class IRPCServiceProvider
    {
    public:
        virtual void Service(uint32_t procedure_id,
                             std::vector<char>& res1, size_t res1_maxsize,
                             std::vector<char>& res2, size_t res2_maxsize,
                             const std::vector<char>& arg1, 
                             const std::vector<char>& arg2, 
                             uint32_t arg3, uint32_t arg4) = 0;

        virtual std::string GetName() = 0;
        virtual ~IRPCServiceProvider() {};
    };
   
    class RPCInterface : public Object, public IIOBusClient
    {
        struct IncomingRequest
        {
            uint32_t                procedure_id;
            uint32_t                extra_arg1;
            uint32_t                extra_arg2;
            IODeviceID              dca_device_id;
            MemAddr                 arg1_base_address;
            MemSize                 arg1_size;
            MemAddr                 arg2_base_address;
            MemSize                 arg2_size;
            MemAddr                 res1_base_address;
            MemAddr                 res2_base_address;
            IONotificationChannelID notification_channel_id;
            Integer                 completion_tag;
        };

        struct ProcessRequest
        {
            uint32_t                procedure_id;
            uint32_t                extra_arg1;
            uint32_t                extra_arg2;
            IODeviceID              dca_device_id;
            MemAddr                 res1_base_address;
            MemAddr                 res2_base_address;
            IONotificationChannelID notification_channel_id;
            Integer                 completion_tag;
            std::vector<char>       data1;
            std::vector<char>       data2;
        };
            
        struct ProcessResponse
        {
            IODeviceID              dca_device_id;
            MemAddr                 res1_base_address;
            MemAddr                 res2_base_address;
            IONotificationChannelID notification_channel_id;
            Integer                 completion_tag;
            std::vector<char>       data1;
            std::vector<char>       data2;
        };

        struct CompletionNotificationRequest
        {
            IONotificationChannelID notification_channel_id;
            Integer                 completion_tag;
        };

        enum ArgumentFetchState
        {
            ARGFETCH_READING1,
            ARGFETCH_READING2,
            ARGFETCH_FINALIZE,
        };

        enum ResponseWritebackState
        {
            RESULTWB_WRITING1,
            RESULTWB_WRITING2,
            RESULTWB_BARRIER,
            RESULTWB_FINALIZE,
        };


        IIOBus&                 m_iobus;
        IODeviceID              m_devid;

        MemSize                 m_lineSize;

        IncomingRequest         m_inputLatch;

        ArgumentFetchState      m_fetchState;
        MemAddr                 m_currentArgumentOffset;
        size_t                  m_numPendingDCAReads;
        size_t                  m_maxArg1Size;
        size_t                  m_maxArg2Size;
        size_t                  m_maxRes1Size;
        size_t                  m_maxRes2Size;
        std::vector<char>       m_currentArgData1;
        std::vector<char>       m_currentArgData2;

        ResponseWritebackState  m_writebackState;
        MemAddr                 m_currentResponseOffset;

        SingleFlag              m_queueEnabled;
        Buffer<IncomingRequest> m_incoming;
        Buffer<ProcessRequest>  m_ready;
        Buffer<ProcessResponse> m_completed;
        Buffer<CompletionNotificationRequest> m_notifications;

        IRPCServiceProvider&    m_provider;

    public:

        RPCInterface(const std::string& name, Object& parent, IIOBus& iobus, IODeviceID devid, Config& config, IRPCServiceProvider& provider);

        Process p_queue;
        Result  DoQueue();
        
        Process p_argumentFetch;
        Result  DoArgumentFetch();       

        Process p_processRequests;
        Result  DoProcessRequests();       

        Process p_writeResponse;
        Result  DoWriteResponse();
        
        Process p_sendCompletionNotifications;
        Result  DoSendCompletionNotifications();

        // from IIOBusClient
        bool OnReadRequestReceived(IODeviceID from, MemAddr address, MemSize size);
        bool OnWriteRequestReceived(IODeviceID from, MemAddr address, const IOData& iodata);
        bool OnReadResponseReceived(IODeviceID from, MemAddr address, const IOData& iodata);

        void GetDeviceIdentity(IODeviceIdentification& id) const;        
        std::string GetIODeviceName() const { return GetFQN(); }
    };

}


#endif
