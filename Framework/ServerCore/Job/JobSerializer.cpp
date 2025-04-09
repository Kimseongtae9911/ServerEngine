#include "../pch.h"

void PacketDispatcher::ProcessJob()
{
	while (true)
	{
		PacketSessionRef session;
		if (!m_jobQueue.Pop(session))
			continue;

		//tbb::hash_map은 자체적으로 lock을 사용하기 때문에 최소화하기 위해 아래와 같이 구현
		auto id = session->GetId();
		if (session->IsDisconnected())
		{
			m_sessionContainer.erase(id);
			continue;
		}
		else
		{
			accessor acc;
			if (!m_sessionContainer.find(acc, id))
				m_sessionContainer.emplace(id, session);
		}

		session->ProcessPacket();
	}
}

void PacketDispatcher::PushJob(PacketSessionRef _session, PacketHeader* _header)
{
	// _session에 pkt를 pushJob
	_session->PushHandler(_header);

	accessor acc;
	if (!m_sessionContainer.find(acc, _session->GetId()))
		m_jobQueue.PushJob(_session);
}