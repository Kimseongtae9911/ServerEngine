#pragma once

extern class ThreadManager* GThreadManager;
extern class MemoryManager* GMemoryManager;
extern class SendBufferPool* GSendBufferPool;
extern class PacketDispatcher* GPacketQueue;

#if _DEBUG
extern class DeadLockProfiler* GDeadLockProfiler;
#endif