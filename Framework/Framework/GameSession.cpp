#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"

GameSession::~GameSession()
{
	CLDebug("GameSession Release");
}

void GameSession::OnConnected()
{
	GameSessionMgr->AddSession(static_pointer_cast<GameSession>(shared_from_this()));

	InitializeQueue();
}

void GameSession::OnDisconnected()
{
	GameSessionMgr->RemoveSession(static_pointer_cast<GameSession>(shared_from_this()));
	__super::OnDisconnected();
}

void GameSession::ParsePacket(uint8* _buffer, int32 _len)
{
	PacketSessionRef session = GetPacketSessionRef();	

	PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);
	GPacketQueue->PushJob(session, header);
}

void GameSession::OnSendPacket(int32 _len)
{
	CLInfo("Send Packet. Len={}", _len);
}
