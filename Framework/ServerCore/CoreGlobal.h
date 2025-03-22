#pragma once

extern class ThreadManager* GThreadManager;
extern class MemoryManager* GMemoryManager;
extern class SendBufferPool* GSendBufferPool;

#if _DEBUG
extern class DeadLockProfiler* GDeadLockProfiler;
#endif