#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const std::string _name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(_name);
#endif

	//������ �����尡 �����ϰ� �ִٸ� ����
	const uint32 lockThreadId = (m_lockFlag.load() & WriteThreadMask) >> 16;
	if (LThreadId == lockThreadId) {
		m_writeCount++;
		return;
	}

	// �����Ͽ� ������ ȹ��
	const uint32 desired = ((LThreadId << 16) & WriteThreadMask);
	const uint64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCnt = 0; spinCnt < MaxSpinCount; ++spinCnt) {
			uint32 expected = EmptyFlag;
			if (m_lockFlag.compare_exchange_strong(OUT expected, desired)) {
				++m_writeCount;	//lock�� ����ٸ� cnt�� ������Ű�� return
				return;
			}
		}

		// �ʹ� ���� �ð��� �ɸ��� �ȵȴ�... ����� ũ����
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

	// ReadLock�� ��� Ǯ��� WriteLock ���� ����
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

	//������ �����尡 �����ϰ� �ִٸ� ����
	const uint32 lockThreadId = (m_lockFlag.load() & ReadCountMask) >> 16;
	if (LThreadId == lockThreadId) {
		m_lockFlag.fetch_add(1);
		return;
	}

	// �����Ͽ� ������(����) ȹ��
	const uint64 beginTick = ::GetTickCount64();
	while (true) {
		for (uint32 spinCnt = 0; spinCnt < MaxSpinCount; ++spinCnt) {
			uint32 expected = (m_lockFlag.load() & ReadCountMask);
			if (m_lockFlag.compare_exchange_strong(OUT expected, expected + 1)) {
				return;
			}
		}

		// �ʹ� ���� �ð��� �ɸ��� �ȵȴ�... ����� ũ����
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
