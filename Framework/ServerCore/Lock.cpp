#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const std::string _name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(_name);
#endif

	//동일한 쓰레드가 소유하고 있다면 성공
	const uint32 lockThreadId = (m_lockFlag.load() & WriteThreadMask) >> 16;
	if (LThreadId == lockThreadId) {
		m_writeCount++;
		return;
	}

	// 경합하여 소유권 획득
	const uint32 desired = ((LThreadId << 16) & WriteThreadMask);
	const uint64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCnt = 0; spinCnt < MaxSpinCount; ++spinCnt) {
			uint32 expected = EmptyFlag;
			if (m_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				++m_writeCount;	//lock을 얻었다면 cnt를 증가시키고 return
				return;
			}
		}

		// 너무 오랜 시간이 걸리면 안된다... 현재는 크래시
		if (::GetTickCount64() - beginTick >= AcquireTimeoutTick)
			CRASH("WriteLock Timeout");

		std::this_thread::yield();
	}
}

void Lock::WriteUnlock(const std::string _name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(_name);
#endif

	// ReadLock을 모두 풀어야 WriteLock 해제 가능
	if ((m_lockFlag.load() & ReadCountMask) != 0)
		CRASH("Invalid unlock order");

	const int32 lockCount = --m_writeCount;
	if (lockCount == 0)
		m_lockFlag.store(EmptyFlag);
}

void Lock::ReadLock(const std::string _name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(_name);
#endif

	//동일한 쓰레드가 소유하고 있다면 성공
	const uint32 lockThreadId = (m_lockFlag.load() & ReadCountMask) >> 16;
	if (LThreadId == lockThreadId) {
		m_lockFlag.fetch_add(1);
		return;
	}

	// 경합하여 소유권(공유) 획득
	const uint64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCnt = 0; spinCnt < MaxSpinCount; ++spinCnt) {
			uint32 expected = (m_lockFlag.load() & ReadCountMask);
			if (m_lockFlag.compare_exchange_strong(OUT expected, expected + 1)) {
				return;
			}
		}

		// 너무 오랜 시간이 걸리면 안된다... 현재는 크래시
		if (::GetTickCount64() - beginTick >= AcquireTimeoutTick)
			CRASH("ReadLock Timeout");

		std::this_thread::yield();
	}
}

void Lock::ReadUnlock(const std::string _name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(_name);
#endif

	if ((m_lockFlag.fetch_sub(1) & ReadCountMask) == 0)
		CRASH("Invalid Read Unlock");
}
