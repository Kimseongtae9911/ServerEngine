#pragma once
#include "Session.h"

class GameSession : public PacketSession
{
public:
	GameSession(tcpSocket _socket, boost::asio::io_context& _context) : PacketSession(std::move(_socket), _context) {}
	virtual ~GameSession();

	void OnConnected() override;
	void OnDisconnected() override;
	void ParsePacket(uint8* _buffer, int32 _len) override;
	void OnSendPacket(int32 _len) override;

public:
	StVector<PlayerRef> m_players;
};

