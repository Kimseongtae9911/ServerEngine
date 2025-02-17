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
			// ���� �ִ� �ٸ� �����尡 �����ϵ��� GlobalQueue�� �ѱ�
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
		//��� �ϰ� ó��
	}
	
	LCurrentJobSerializer = nullptr;
}