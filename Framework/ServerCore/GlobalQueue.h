#pragma once

// 전역으로 잡을 배분하는 큐
class GlobalQueue
{
public:
	GlobalQueue();
	~GlobalQueue();

	void PushJob(JobSerializerRef _jobQueue);
	JobSerializerRef Pop();

private:
	JobQueue<JobSerializerRef> m_jobSerializers;
};

