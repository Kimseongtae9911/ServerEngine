#pragma once

// 잡큐를 사용하는 클래스들은 해당 클래스를 상속
template<typename CallbackType = void()>
class JobSerializer : public std::enable_shared_from_this<JobSerializer<CallbackType>>
{
public:
	using JobRef = std::shared_ptr<class Job<CallbackType>>;
	using TJob = Job<CallbackType>;

	void ExecuteAsync(CallbackType&& _callback)
	{
		PushJob(ObjectPool<TJob>::MakeObject(std::move(_callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void ExecuteAsync(Ret(T::* _memFunc)(Args...), Args... args)
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

class PacketSession;
// todo: 공통 잡디스패쳐를 만들기, PacketDispatcher는 전역으로 사용중이라서 session.h도 모두 include가 필요함
class PacketDispatcher
{
public:
	void PushJob(PacketSessionRef _session, PacketHeader* _header);

	void PushJob(PacketSessionRef _session)
	{
		m_jobQueue.PushJob(_session);
	}

	void ProcessJob();

private:
	JobQueue<PacketSessionRef> m_jobQueue;
	StCUMap<int32, PacketSessionRef> m_sessionContainer;
};

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, uint8*, int32)>;
extern PacketHandlerFunc GPacketHandler[std::numeric_limits<uint16>::max()];

class SessionQueue
{
public:
	void Init(PacketSessionRef _session) { m_owner = _session; }

	void PushJob(PacketHeader* _header)
	{
		m_jobQueue.PushJob(_header);
	}

	void ProcessJob()
	{
		// 해당 세션의 추가 패킷이 와도 현재 잡큐의 있는 개수만큼만 처리
		// 패킷을 처리하는 스레드는 해당 함수를 수행하는 것 뿐..
		auto sessionRef = m_owner.lock();
		if (nullptr == sessionRef)
			return;

		int32 curJobCnt = m_jobQueue.UnSafeSize();
		for (int32 i = 0; i < curJobCnt; ++i)
		{
			PacketHeader* header;
			if (m_jobQueue.Pop(header))
			{
				uint8* buffer = reinterpret_cast<uint8*>(header);
				bool ret = GPacketHandler[header->protocol](sessionRef, buffer, header->size);
			}
		}

		// 다시 PacketDispatcher로 세션 넣기
		GPacketQueue->PushJob(sessionRef);
	}

private:
	JobQueue<PacketHeader*> m_jobQueue;
	std::weak_ptr<PacketSession> m_owner;
};

