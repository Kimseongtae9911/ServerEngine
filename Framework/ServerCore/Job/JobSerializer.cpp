#include "../pch.h"

void PacketDispatcher::ProcessJob()
{
	while (true)
	{
		PacketSessionRef session;
		if (!m_jobQueue.Pop(session))
			continue;

		// 필요한가?
		/*auto id = session->GetId();
		auto it = m_sessionContainer.find(session->GetId());
		if (it == m_sessionContainer.end())
			m_sessionContainer.emplace(session->GetId(), session);*/

		if (session->IsDisconnected())
			continue;

		session->ProcessPacket();
	}
}

void PacketDispatcher::PushJob(PacketSessionRef _session, PacketHeader* _header)
{
	// _session에 pkt를 pushJob
	_session->PushHandler(_header);

	m_jobQueue.PushJob(_session);
}