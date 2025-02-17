#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"
#include "GlobalQueue.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;
SendBufferPool* GSendBufferPool = nullptr;
GlobalQueue* GGlobalQueue = nullptr;

#if _DEBUG
DeadLockProfiler* GDeadLockProfiler = nullptr;
#endif

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryManager = new MemoryManager();
		GSendBufferPool = new SendBufferPool();
		GGlobalQueue = new GlobalQueue();

#if _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;
		delete GSendBufferPool;
		delete GGlobalQueue;

#if _DEBUG
		delete GDeadLockProfiler;
#endif
	}
} CoreGlobal;