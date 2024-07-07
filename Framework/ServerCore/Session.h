#pragma once

class Session : public std::enable_shared_from_this<Session>, public NetworkObject
{
public:
	Session(tcpSocket _socket);
	~Session();

	void RunObject() override;

private:
	void OnDisconnected();

private:
	tcpSocket m_socket;
	std::array<uint8, 1024> m_recvBuffer;
};

