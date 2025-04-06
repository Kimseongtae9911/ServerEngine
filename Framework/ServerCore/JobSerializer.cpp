#include "pch.h"

void PacketDispatcher::ProcessJob()
{
	PacketSessionRef session;
	if (!m_jobQueue.Pop(session))
	{
		return;
	}

	auto id = session->GetId();

	auto it = m_sessionContainer.find(session->GetId());
	if (it == m_sessionContainer.end())
		m_sessionContainer.emplace(session->GetId(), session);
}
