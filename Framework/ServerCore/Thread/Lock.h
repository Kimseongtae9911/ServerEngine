#pragma once

/*
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W: WriteFlag
R: ReadFlag
*/

/*
Flag를 이용한 간단한 RW락..
**주의**
ReadLock을 가진 스레드가 WriteLock을 소유하면 안됨

**성능**
성능 자체는 std::shared_lock이 더 좋다
*/

class Lock
{
	enum : uint32
	{
		AcquireTimeoutTick = 1000,
		MaxSpinCount = 50000,
		WriteThreadMask = 0xFFFF'0000,
		ReadCountMask = 0x0000'FFFF,
		EmptyFlag = 0x0000'0000
	};

public:
	void WriteLock(const std::string _name);
	void WriteUnlock(const std::string _name);
	void ReadLock(const std::string _name);
	void ReadUnlock(const std::string _name);

private:
	Atomic<uint32> m_lockFlag = EmptyFlag;
	uint16 m_writeCount = 0;
};

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const std::string& _name) : m_lock(lock), m_name(m_name) { m_lock.ReadLock(m_name); }
	~ReadLockGuard() { m_lock.ReadUnlock(m_name); }

private:
	Lock& m_lock;
	std::string m_name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const std::string& _name) : m_lock(lock), m_name(_name) { m_lock.WriteLock(m_name); }
	~WriteLockGuard() { m_lock.WriteUnlock(m_name); }

private:
	Lock& m_lock;
	std::string m_name;
};
