#pragma once
#include "Job.h"
#include "JobQueue.h"

// 잡큐를 사용하는 클래스들은 해당 클래스를 상속
template<typename CallbackType = void()>
class JobSerializer : std::enable_shared_from_this<JobSerializer<CallbackType>>
{
public:
	using JobRef = std::shared_ptr<class Job<CallbackType>>;
	using TJob = Job<CallbackType>;

	void ExecuteAsync(CallbackType&& _callback)
	{
		PushJob(ObjectPool<TJob>::MakeObject(std::move(_callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void ExecuteAsync(Ret(T::*_memFunc)(Args...), Args... args)
	{
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(this->shared_from_this());
		PushJob(ObjectPool<TJob>::MakeObject(owner, _memFunc, std::forward<Args>(args)...));
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

template<typename CallbackType>
inline void JobSerializer<CallbackType>::PushJob(JobRef&& _job)
{
	m_jobCount.fetch_add(1);
	m_jobQueue.PushJob(_job);
}

template<typename CallbackType>
inline void JobSerializer<CallbackType>::Execute()
{
	int32 curJobCount = m_jobCount;

	int32 successCnt = 0;
	for (int32 i = 0; i < curJobCount; ++i)
	{
		JobRef job;
		if (m_jobQueue.Pop(job))
		{
			job->Execute();
			++successCnt;
		}
	}

	m_jobCount.fetch_sub(curJobCount);
}

// todo: 공통 잡디스패쳐를 만들기
template<class PacketType>
class PacketDispatcher
{
public:
	void PushJob(PacketSessionRef _session, const PacketType& pkt)
	{
		// _session에 pkt를 pushJob
		_session->PushHandler(pkt);

		m_jobQueue.PushJob(_session);
	}

	void ProcessJob()
	{
		PacketSessionRef session;
		if (!m_jobQueue.Pop(session))
		{
			return;
		}

		auto it = m_sessionContainer.find(session->GetId());
		if (it == m_sessionContainer.end())
			m_sessionContainer.emplace(session->GetId(), session);	
	}

private:
	JobQueue<PacketSessionRef> m_jobQueue;
	StCUMap<int32, PacketSessionRef> m_sessionContainer;
};

template<class PacketType>
class SessionQueue
{
	void PushJob(PacketSessionRef _session, const PacketType& pkt)
	{
		// _session에 pkt를 pushJob
		_session->PushHandler(pkt);

		m_jobQueue.PushJob(_session);
	}

	void ProcessJob()
	{

	}

private:
	JobQueue<PacketType> m_jobQueue;
};
