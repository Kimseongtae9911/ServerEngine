#pragma once
#include "NetworkInterface.h"
#include "NetAddress.h"

class Service;

class Session : public std::enable_shared_from_this<Session>, public NetworkObject
{
public:
	Session(tcpSocket _socket, boost::asio::io_context& _context);
	~Session();

	void RunObject() override;
	void SendPacket(int8* _data, int32 _length) override;

	void SetService(Service* _service) { m_service = _service; }
	NetAddress GetNetAddress() const { return NetAddress(m_socket.remote_endpoint()); }

protected:
	void OnDisconnected();
	void OnSendPacket(int32 _length) override;

protected:
	tcpSocket m_socket;
	Service* m_service = nullptr;
	NetAddress m_netAddress;
	
	std::array<uint8, 1024> m_recvBuffer;
	std::array<uint8, 1024> m_sendBuffer;
	boost::asio::io_service::strand m_strand;
};

