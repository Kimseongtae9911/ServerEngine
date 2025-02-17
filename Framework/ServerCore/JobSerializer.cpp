#include "pch.h"
#include "JobSerializer.h"
#include "GlobalQueue.h"

void JobSerializer::PushJob(JobRef&& _job)
{
	const int32 prevCount = m_jobCount.fetch_add(1);
	m_jobQueue.PushJob(_job);

	if (prevCount == 0)
	{
		if (LCurrentJobSerializer == nullptr)
		{
			Execute();
		}
		else
		{
			// 여유 있는 다른 스레드가 실행하도록 GlobalQueue로 넘김
			GGlobalQueue->PushJob(shared_from_this());
		}
	}
}

void JobSerializer::Execute()
{
	LCurrentJobSerializer = this;

	while (true)
	{
		StVector<JobRef> jobs;
		//모든 일감 처리
	}
	
	LCurrentJobSerializer = nullptr;
}