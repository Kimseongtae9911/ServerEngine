#pragma once

template<class T>
class JobQueue
{
public:
	void PushJob(T _job)
	{
		m_jobs.push(_job);
	}

	T Pop()
	{
		if (m_jobs.empty())
			return T();

		T job;
		if (m_jobs.try_pop(job))
		{
			return job;
		}

		return T();
	}	

	void Clear()
	{
		m_jobs.clear();
	}


private:
	tbb::concurrent_queue<T> m_jobs;
};

