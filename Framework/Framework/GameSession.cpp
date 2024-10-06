#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"

GameSession::~GameSession()
{
	CLDebug("GameSession Release");
}

void GameSession::OnConnected()
{
	GameSessionMgr->AddSession(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	GameSessionMgr->RemoveSession(static_pointer_cast<GameSession>(shared_from_this()));
	__super::OnDisconnected();
}

void GameSession::ParsePacket(uint8* _buffer, int32 _len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);

	CLInfo("Recv Packet. Protocol= {}, Len={}", header->protocol, header->size);
}

void GameSession::OnSendPacket(int32 _len)
{
	CLInfo("Send Packet. Len={}", _len);
}
