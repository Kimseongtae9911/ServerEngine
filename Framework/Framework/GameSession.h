#pragma once
#include "Session.h"

class GameSession : public Session
{
public:
	GameSession(tcpSocket _socket, boost::asio::io_context& _context) : Session(std::move(_socket), _context) {}
	~GameSession();

	void OnConnected() override;
	void OnDisconnected() override;
	int32 ProcessPacket(uint8* _buffer, int32 _len) override;
	void OnSendPacket(int32 _len) override;
};

