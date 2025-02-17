#pragma once

extern class ThreadManager* GThreadManager;
extern class MemoryManager* GMemoryManager;
extern class SendBufferPool* GSendBufferPool;
extern class GlobalQueue* GGlobalQueue;

#if _DEBUG
extern class DeadLockProfiler* GDeadLockProfiler;
#endif