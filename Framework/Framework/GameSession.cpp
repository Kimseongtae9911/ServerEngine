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

int32 GameSession::ProcessPacket(uint8* _buffer, int32 _len)
{
	CLInfo("Recv Packet. Len={}", _len);

	auto sendBuffer = GSendBufferPool->UseChunk(4096);
	::memcpy_s(sendBuffer->GetBuffer(), _len, _buffer, _len);
	sendBuffer->Close(_len);

	GameSessionMgr->Broadcast(sendBuffer);

	return _len;
}

void GameSession::OnSendPacket(int32 _len)
{
	CLInfo("Send Packet. Len={}", _len);
}
