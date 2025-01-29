#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

Room GRoom;

void Room::EnterRoom(PlayerRef _player)
{
	WRITE_LOCK;
	m_players[_player->playerId] = _player;
}

void Room::LeaveRoom(PlayerRef _player)
{
	WRITE_LOCK;
	m_players.erase(_player->playerId);
}

void Room::BroadcastPacket(SendBufRef _sendBuffer)
{
	WRITE_LOCK;
	for (auto& [id, player] : m_players)
	{
		player->ownerSession->SendPacket(_sendBuffer);
	}
}
