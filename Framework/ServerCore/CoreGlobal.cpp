#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;
SendBufferPool* GSendBufferPool = nullptr;
PacketDispatcher* GPacketQueue = nullptr;

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
		GPacketQueue = new PacketDispatcher();

#if _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;
		delete GSendBufferPool;
		delete GPacketQueue;

#if _DEBUG
		delete GDeadLockProfiler;
#endif
	}
} CoreGlobal;