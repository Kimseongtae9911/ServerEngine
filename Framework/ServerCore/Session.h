#pragma once
#include "NetworkInterface.h"

class Service;

class Session : public std::enable_shared_from_this<Session>, public NetworkObject
{
public:
	Session(tcpSocket _socket, boost::asio::io_context& _context);
	~Session();

	void RunObject() override;
	void SendPacket(int8* _data, int32 _length) override;

	void SetService(std::shared_ptr<Service> _service) { m_service = _service; }

protected:
	void OnDisconnected();
	void OnSendPacket(int32 _length) override;

protected:
	tcpSocket m_socket;
	std::shared_ptr<Service> m_service;
	
	std::array<uint8, 1024> m_recvBuffer;
	std::array<uint8, 1024> m_sendBuffer;
	boost::asio::io_service::strand m_strand;
};

