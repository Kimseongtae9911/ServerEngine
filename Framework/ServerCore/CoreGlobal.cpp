#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;

#if _DEBUG
DeadLockProfiler* GDeadLockProfiler = nullptr;
#endif

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();

#if _DEBUG
		GDeadLockProfiler = new DeadLockProfiler();
#endif
	}
	~CoreGlobal()
	{
		delete GThreadManager;
		
#if _DEBUG
		delete GDeadLockProfiler;
#endif
	}
} CoreGlobal;