#pragma once

template<class T>
class JobQueue
{
public:
	void PushJob(T _job)
	{
		m_jobs.push(_job);
	}

	bool Pop(T& _job)
	{
		if (m_jobs.empty())
			return false;

		T job;
		if (m_jobs.try_pop(job))
		{
			_job = std::move(job);
			return true;
		}

		return false;
	}	

	void Clear()
	{
		m_jobs.clear();
	}

	int32 UnSafeSize() { return m_jobs.unsafe_size(); }

private:
	tbb::concurrent_queue<T> m_jobs;
};

