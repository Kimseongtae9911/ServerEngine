#pragma once

// �������� ���� ����ϴ� ť
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

