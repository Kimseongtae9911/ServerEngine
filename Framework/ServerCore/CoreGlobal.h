#pragma once

extern class ThreadManager* GThreadManager;

#if _DEBUG
extern class DeadLockProfiler* GDeadLockProfiler;
#endif