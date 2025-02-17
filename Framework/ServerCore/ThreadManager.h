#pragma once

#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void RunThreads(std::function<void(void)> _callback);
	void JoinThreads();

	static void InitTLS();
	static void DestroyTLS();
		
	static void DoGlobalQueueWork();

private:
	Mutex	m_lock;
	std::vector<std::thread> m_threads;
};

