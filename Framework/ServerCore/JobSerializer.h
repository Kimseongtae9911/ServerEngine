#pragma once
#include "Job.h"
#include "JobQueue.h"

// 잡큐를 사용하는 클래스들은 해당 클래스를 상속
class JobSerializer : std::enable_shared_from_this<JobSerializer>
{
public:
	void ExecuteAsync(CallbackType&& _callback)
	{
		PushJob(ObjectPool<Job>::MakeObject(std::move(_callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void ExecuteAsync(Ret(T::*_memFunc)(Args...), Args... args)
	{
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		PushJob(ObjectPool<Job>::MakeObject(owner, _memFunc, std::forward<Args>(args)...));
	}

	void ClearJobs() { m_jobQueue.Clear(); }

private:
	void PushJob(JobRef&& _job);

public:
	void Execute();

protected:
	JobQueue<JobRef> m_jobQueue;
	Atomic<int32> m_jobCount = 0;
};

