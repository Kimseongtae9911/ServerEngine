#pragma once
#include "Job.h"
#include "JobQueue.h"
#include "Session.h"

// ��ť�� ����ϴ� Ŭ�������� �ش� Ŭ������ ���
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

// todo: ���� ������ĸ� �����, PacketDispatcher�� �������� ������̶� session.h�� ��� include�� �ʿ���
class PacketDispatcher
{
public:
	void PushJob(PacketSessionRef _session, PacketHeader* _header)
	{
		// _session�� pkt�� pushJob
		//_session->PushHandler(_header);

		m_jobQueue.PushJob(_session);
	}

	void PushJob(PacketSessionRef _session)
	{
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
		// �ش� ������ �߰� ��Ŷ�� �͵� ���� ��ť�� �ִ� ������ŭ�� ó��
		// ��Ŷ�� ó���ϴ� ������� �ش� �Լ��� �����ϴ� �� ��..
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
				GPacketHandler[header->protocol](sessionRef, buffer, header->size);
			}
		}

		// �ٽ� PacketDispatcher�� ���� �ֱ�
	}

private:
	JobQueue<PacketHeader*> m_jobQueue; 
	std::weak_ptr<PacketSession> m_owner;
};
