#pragma once
#include "JobSerializer.h"

class Room : public JobSerializer
{
public:
	void EnterRoom(PlayerRef _player);
	void LeaveRoom(PlayerRef _player);
	void BroadcastPacket(SendBufRef _sendBuffer);

private:
	std::map<uint64, PlayerRef> m_players;
};

extern std::shared_ptr<Room> GRoom;