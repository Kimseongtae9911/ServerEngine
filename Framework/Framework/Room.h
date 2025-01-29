#pragma once
class Room
{
public:
	void EnterRoom(PlayerRef _player);
	void LeaveRoom(PlayerRef _player);
	void BroadcastPacket(SendBufRef _sendBuffer);


private:
	USE_LOCK;
	std::map<uint64, PlayerRef> m_players;
};

extern Room GRoom;