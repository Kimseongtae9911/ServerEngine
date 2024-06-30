#pragma once

#define OUT

#define _WINDOW _WIN32 | _WIN64

/*----------------
	  Lock
----------------*/

#define USE_MANY_LOCKS(count) Lock _locks[count];
#define USE_LOCK USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx) ReadLockGuard readLockGuard_##idx(_locks[idx], std::string(typeid(this).name()));
#define READ_LOCK READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx) WriteLockGuard writeLockGuard_##idx(_locks[idx], std::string(typeid(this).name()));
#define WRITE_LOCK WRITE_LOCK_IDX(0)

/*----------------
	  Memory
----------------*/
#ifdef _DEBUG
#define stalloc(size) PoolAllocator::Alloc(size)
#define strelease(ptr) PoolAllocator::Release(ptr)
#else
#define stalloc(size) BaseAllocator::Alloc(size)
#define strelease(ptr) BaseAllocator::Release(ptr)
#endif

/*----------------
	  Crash
----------------*/

#define CRASH(cause)						\
{											\
	uint32_t* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr))							\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}