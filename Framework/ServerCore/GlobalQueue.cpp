#include "pch.h"
#include "GlobalQueue.h"

GlobalQueue::GlobalQueue()
{
}

GlobalQueue::~GlobalQueue()
{
}

void GlobalQueue::PushJob(JobSerializerRef _jobQueue)
{
	m_jobSerializers.PushJob(_jobQueue);
}

JobSerializerRef GlobalQueue::Pop()
{
	return m_jobSerializers.Pop();
}
