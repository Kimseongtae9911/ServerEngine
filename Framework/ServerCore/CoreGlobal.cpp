#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"
#include "MemoryManager.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;

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

#if _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryManager;

#if _DEBUG
		delete GDeadLockProfiler;
#endif
	}
} CoreGlobal;