#pragma once
#include "NetAddress.h"

class Service;

class Session : public std::enable_shared_from_this<Session>
{
	friend class Acceptor;
	friend class Service;

public:
	Session(tcpSocket _socket, boost::asio::io_context& _context);
	~Session();

	void SendPacket(int8* _data, int32 _length);
	
	void ProcessRecv();
	void ProcessConnected();
	void DisconnectSession(EResultCode _resultCode);

	void SetService(Service* _service) { m_service = _service; }
	NetAddress GetNetAddress() const { return NetAddress(m_socket.remote_endpoint()); }

protected:
	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual void OnSendPacket(int32 _length);
	virtual void OnRecvPacket(std::array<uint8, 1024> _buffer, int32 _len);

protected:
	tcpSocket m_socket;
	Service* m_service = nullptr;
	NetAddress m_netAddress;
	
	std::array<uint8, 1024> m_recvBuffer;
	std::array<uint8, 1024> m_sendBuffer;
	boost::asio::io_service::strand m_strand;
};

