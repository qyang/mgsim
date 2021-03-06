#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#ifndef PROCESSOR_H
#error This file should be included in Processor.h
#endif

// A list of dependencies that prevent a family from being
// terminated or cleaned up
enum FamilyDependency
{
    FAMDEP_THREAD_COUNT,        // Number of allocated threads
    FAMDEP_OUTSTANDING_READS,   // Number of outstanding memory reads
    FAMDEP_PREV_SYNCHRONIZED,   // Family has synchronized on the previous processor
    FAMDEP_SYNC_SENT,           // The synchronization has been sent
    FAMDEP_DETACHED,            // Family has been detached
    FAMDEP_ALLOCATION_DONE,     // Thread allocation is done
};

// A list of dependencies that prevent a thread from being
// terminated or cleaned up
enum ThreadDependency
{
    THREADDEP_OUTSTANDING_WRITES,   // Number of outstanding memory writes
    THREADDEP_PREV_CLEANED_UP,      // Predecessor has been cleaned up
    THREADDEP_TERMINATED,           // Thread has terminated
};

class Allocator : public Object, public Inspect::Interface<Inspect::Read>
{
    friend class Processor;

public:
    typedef LinkedList< TID, ThreadTable, &Thread::next> ThreadList;
   
	struct AllocRequest
	{
	    LFID           first_fid;      ///< FID of the family on the first core
	    LFID           prev_fid;       ///< FID of the family on the previous core
	    PSize          placeSize;      ///< Number of cores to allocate on
	    AllocationType type;           ///< Type of the allocation
	    PID            completion_pid; ///< Core that requested the allocation
	    RegIndex       completion_reg; ///< Register (on that core) that will receive the FID
	    bool      	   bundle;         ///< Whether the family parameters are already bundled.
        size_t         priority;       ///< Preset priority for RT
	    Bundle         binfo;          ///< The bundle information for bundled requests.
	};

    // These are the different states in the state machine for
    // family creation
	enum CreateState
	{
		CREATE_INITIAL,             // Waiting for a family to create
	        CREATE_LOAD_REGSPEC,        // Load program code to look for register window specification
		CREATE_LOADING_LINE,        // Waiting until the cache-line is loaded
		CREATE_LINE_LOADED,         // The line has been loaded
		CREATE_RESTRICTING,         // Check family property and restrict if necessary
		CREATE_ALLOCATING_REGISTERS,// Allocating register space
		CREATE_BROADCASTING_CREATE, // Broadcasting the create
		CREATE_ACTIVATING_FAMILY,   // Activating the family
		CREATE_NOTIFY,              // Notifying creator
	};
	
	struct BundleInfo
	{
		MemAddr   addr;            ///< Memory Entry
		Integer   parameter;      ///< Parameter for shareds
        size_t    priority;        ///< Preset priority for RT
		RegIndex  completion_reg; ///< Register (on that core) that will receive the FID
	};
	
	enum BundleState
	{
		BUNDLE_INITIAL,             // Waiting for a system call to be handle
		BUNDLE_LOADING_LINE,        // Waiting until the cache-line is loaded
		BUNDLE_LINE_LOADED,         // The line has been loaded        
	};   

    Allocator(const std::string& name, Processor& parent, Clock& clock,
              FamilyTable& familyTable, ThreadTable& threadTable, RegisterFile& registerFile, RAUnit& raunit, ICache& icache, DCache& dcache, Network& network, Pipeline& pipeline,
              Config& config);
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;

    // Allocates the initial family consisting of a single thread on the first CPU.
    // Typically called before tha actual simulation starts.
    void AllocateInitialFamily(MemAddr pc, bool legacy, PSize placeSize, SInteger startIndex);

    /// Allocates a contexts and sets the family's 'link' field to prev_fid
    LFID AllocateContext(ContextType type, size_t prioriy, LFID prev_fid, PSize placeSize);

    // Returns the physical register address for a logical register in a certain family.
    RegAddr GetRemoteRegisterAddress(LFID fid, RemoteRegType kind, const RegAddr& addr) const;

    Family& GetFamilyChecked(LFID fid, FCapability capability) const;

    //
    // Thread management
    //
    bool PrepareThreads(const ThreadQueue& threads);   //  Preapare ins. for the specified threads
   // bool ActivateThreads(const ThreadQueue& threads);   // Activates the specified threads
    bool RescheduleThread(TID tid, MemAddr pc);         // Reschedules a thread from the pipeline
    bool SuspendThread(TID tid, MemAddr pc);            // Suspends a thread at the specified PC
    bool KillThread(TID tid);                           // Kills a thread

    bool QueueFamilyAllocation(const RemoteMessage& msg, bool bundle);
    bool QueueFamilyAllocation(const LinkMessage& msg);
    bool QueueBundle(const MemAddr addr, Integer parameter, RegIndex completion_reg);
    bool ActivateFamily(LFID fid);

    FCapability InitializeFamily(LFID fid) const;
    void ReleaseContext(LFID fid);

    bool QueueCreate(const RemoteMessage& msg, PID src);
    bool QueueCreate(const LinkMessage& msg);
    //bool QueueReadyThreads(const ThreadQueue& threads);
    bool QueueActiveThreads(const ThreadQueue& threads, Integer priority);
    bool QueueThreads(ThreadList& list, const ThreadQueue& threads, ThreadState state);

    bool OnICachelineLoaded(CID cid);
    bool OnDCachelineLoaded(char* data);
    bool OnMemoryRead(LFID fid);

    bool DecreaseFamilyDependency(LFID fid, FamilyDependency dep);
    bool DecreaseFamilyDependency(LFID fid, Family& family, FamilyDependency dep);
    bool IncreaseThreadDependency(TID tid, ThreadDependency dep);
    bool DecreaseThreadDependency(TID tid, ThreadDependency dep);

    TID PopActiveThread();

    // Helpers
    TID  GetRegisterType(LFID fid, RegAddr addr, RegClass* group, size_t *rel) const;

    // Debugging
    void Cmd_Info(std::ostream& out, const std::vector<std::string>& arguments) const override;
    void Cmd_Read(std::ostream& out, const std::vector<std::string>& arguments) const override;

private:
    struct CreateInfo
    {
        LFID     fid;
        PID      completion_pid;
        RegIndex completion_reg;
        Integer  parameter; // For bundle creation
        SInteger index;     // For bundle creation
        bool     bundle;    // For bundle creation
    };

    // A queued integer register write
    struct RegisterWrite
    {
        RegIndex address;   // Where to write
        Integer  value;     // What to write
    };


    Integer CalculateThreadCount(SInteger start, SInteger limit, SInteger step);
    void    CalculateDistribution(Family& family, Integer nThreads, PSize numCores);
    bool    AllocateRegisters(LFID fid, ContextType type);
    bool    AllocateThread(LFID fid, TID tid, bool isNewlyAllocated = true);
    bool    PushCleanup(TID tid);
    bool    IsContextAvailable(ContextType type) const;

    // Thread queue manipulation
    void Push(ThreadQueue& queue, TID tid);
    TID  Pop (ThreadQueue& queue);

    Processor&    m_parent;
    FamilyTable&  m_familyTable;
    ThreadTable&  m_threadTable;
    RegisterFile& m_registerFile;
    RAUnit&       m_raunit;
    ICache&       m_icache;
    DCache&       m_dcache;
    Network&      m_network;
	Pipeline&	  m_pipeline;
    
    
    size_t        m_priorities;
    size_t        m_pribits;
    char          m_bundleData[MAX_MEMORY_OPERATION_SIZE];

    Buffer<BundleInfo>    m_bundle;
   
    Buffer<TID>           m_cleanup;                 ///< Cleanup queue
    Buffer<CreateInfo>*   m_currentCreation;         ///< current create queue
    const CreateInfo*     m_createInfo;              ///< current create info
    CreateState           m_createState;	         ///< State of the current state;
    CID                   m_createLine;	   	         ///< Cache line that holds the register info
    ThreadList            m_preparingThreads1;           ///< Queue of the threads can be ready; from the pipeline
    ThreadList            m_preparingThreads2;           ///< Queue of the threads can be ready; from the rest
    ThreadList*           m_prevPreparingList;           ///< Which ready list was used last cycle. For round-robin prioritization.
   
    BundleState           m_bundleState;
   
    // The family allocation request queues
    std::vector<Buffer<AllocRequest>*> m_allocRequestsSuspend; ///< Non-exclusive requests that want to suspend.
    std::vector<Buffer<AllocRequest>*> m_allocRequestsNoSuspend;	 ///< Non-exclusive requests that do not want to suspend.
    std::vector<Buffer<AllocRequest>*> m_allocRequestsExclusive;  ///< Exclusive requests.
    
    std::vector<Buffer<CreateInfo>*>  m_creates;                 ///< Create queue
    
    std::vector<Buffer<LFID>*>        m_allocThreads;      ///< This is the queue of families waiting for initial thread allocation
    
    Result DoThreadAllocate();
    Result DoFamilyAllocate();
    Result DoFamilyCreate();
    Result DoThreadPreparation();
    Result DoBundle();

    // Statistics
    CycleNo    m_lastcycle;
    BufferSize m_maxallocex;
    BufferSize m_totalallocex;
    BufferSize m_curallocex;
    FSize      m_numCreatedFamilies;
    TSize      m_numCreatedThreads;
    void       UpdateStats();

public:
    // Processes
    Process p_ThreadAllocate;
    Process p_FamilyAllocate;
    Process p_FamilyCreate;
    Process p_ThreadPrepare;
  //  Process p_ThreadActivation;
    Process p_Bundle;

    ArbitratedService<>      p_allocFamily;     ///< Arbitrator for Family allocation
    ArbitratedService<>      p_allocThreads;    ///< Arbitrator for thread allocation
    ArbitratedService<>      p_prepThreads;   ///< Arbitrator for m_preparingThreads2
    ArbitratedService<>      p_readyThreads;  ///< Arbitrator for m_readyThreads
    std::vector<ThreadList*>  m_activeThreads;  ///< Queue of the active threads

    size_t                m_numThreadsPerState[TST_NUMSTATES]; ///< For debugging only.   
        
    // Statistics
    BufferSize GetTotalAllocatedEx() { UpdateStats(); return m_totalallocex; }
    BufferSize GetMaxAllocatedEx() const { return m_maxallocex; }
    TSize GetTotalFamiliesCreated() const { return m_numCreatedFamilies; }
    FSize GetTotalThreadsCreated() const { return m_numCreatedThreads; }
    
};

#endif
