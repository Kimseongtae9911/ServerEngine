#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	JoinThreads();
}

void ThreadManager::RunThreads(std::function<void(void)> _callback)
{
	LockGuard guard(m_lock);

	m_threads.push_back(std::thread([=]()
		{
			InitTLS();
			_callback();
			DestroyTLS();
		}));
}

void ThreadManager::JoinThreads()
{
	for (auto& th : m_threads)
		if (th.joinable())
			th.join();

	m_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{

}
