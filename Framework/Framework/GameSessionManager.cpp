#include "pch.h"
#include "GameSessionManager.h"
#include "GameSession.h"

GameSessionManager* GameSessionManager::s_instance;

void GameSessionManager::AddSession(GameSessionRef _session)
{
	WRITE_LOCK;
	m_sessions.insert(_session);
}

void GameSessionManager::RemoveSession(GameSessionRef _session)
{
	WRITE_LOCK;
	m_sessions.erase(_session);
}

void GameSessionManager::Broadcast(SendBufRef _sendBuffer)
{
	StSet<GameSessionRef> sessions;
	{
		READ_LOCK;
		sessions = m_sessions;
	}

	for (auto session : sessions)
		session->SendPacket(_sendBuffer, false);
}
