#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"

std::shared_ptr<Room> GRoom;

void Room::EnterRoom(PlayerRef _player)
{
	m_players[_player->playerId] = _player;
}

void Room::LeaveRoom(PlayerRef _player)
{
	m_players.erase(_player->playerId);
}

void Room::BroadcastPacket(SendBufRef _sendBuffer)
{	
	for (auto& [id, player] : m_players)
	{
		player->ownerSession->SendPacket(_sendBuffer);
	}
}
//
//void Room::FlushJob()
//{
//	while (true)
//	{
//		JobRef job = m_jobQueue.Pop();
//		if (job == nullptr)
//			break;
//
//		job->Execute();
//	}
//}
