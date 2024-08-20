#pragma once
#include "NetworkInterface.h"

class Session : public std::enable_shared_from_this<Session>, public NetworkObject
{
public:
	Session(tcpSocket _socket, boost::asio::io_context& _context);
	~Session();

	void RunObject() override;
	void SendPacket(int8* _data, int32 _length) override;

private:
	void OnDisconnected();
	void OnSendPacket(int32 _length) override;

private:
	tcpSocket m_socket;
	
	std::array<uint8, 1024> m_recvBuffer;
	std::array<uint8, 1024> m_sendBuffer;
	boost::asio::io_service::strand m_strand;
};

