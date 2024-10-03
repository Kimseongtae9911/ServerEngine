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
	for (auto session : m_sessions)
		session->SendPacket(_sendBuffer, false);
}
