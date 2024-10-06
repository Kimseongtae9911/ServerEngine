#pragma once
#include "NetAddress.h"
#include "RecvBuffer.h"

class Service;

class Session : public std::enable_shared_from_this<Session>
{
	friend class Acceptor;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000 //64KB
	};

public:
	Session(tcpSocket _socket, boost::asio::io_context& _context);
	~Session();

	void SetId(int32 _id) { m_id = _id; }

	void SendPacket(SendBufRef _sendBuffer, bool _immediately = true);
	bool Connect(const NetAddress& _netAddress);
	
	void ProcessRecv();
	void ProcessConnected();
	void DisconnectSession(EResultCode _resultCode);

	void SetService(Service* _service) { m_service = _service; }
	NetAddress GetNetAddress() const { return NetAddress(m_socket.remote_endpoint()); }

	virtual void OnTimer();

protected:
	void OnRecvPacket(int32 _len);

	virtual void OnConnected();
	virtual void OnDisconnected();
	virtual void OnSendPacket(int32 _length);
	virtual int32 ProcessPacket(uint8* _buffer, int32 _len) sealed;
	virtual void ParsePacket(uint8* _buffer, int32 _len) abstract;

	void FlushSendQueue();

protected:
	tcpSocket m_socket;
	Service* m_service = nullptr;
	NetAddress m_netAddress;
	
	RecvBuffer m_recvBuffer;
	boost::asio::io_service::strand m_strand;
	tbb::concurrent_queue<SendBufRef> m_sendQueue;

	std::atomic_bool m_isDisconnected = false;
	
	int32 m_id = 0;
};