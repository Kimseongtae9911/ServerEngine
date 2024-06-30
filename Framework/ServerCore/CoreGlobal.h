#pragma once

extern class ThreadManager* GThreadManager;
extern class MemoryManager* GMemoryManager;

#if _DEBUG
extern class DeadLockProfiler* GDeadLockProfiler;
#endif