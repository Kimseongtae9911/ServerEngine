#include "pch.h"
#include "NetworkCore.h"
#include "Session.h"
#include "SocketUtils.h"
#include "ThreadManager.h"
#include "Service.h"

class GameSession : public Session
{
public:
	GameSession(tcpSocket _socket, boost::asio::io_context& _context) : Session(std::move(_socket), _context) {}

	~GameSession() { CLInfo("GameSession Release"); }

	int32 ProcessPacket(uint8* _buffer, int32 _len) override
	{
		CLInfo("Recv Packet. Len={}", _len);

		SendPacket(_buffer, _len);

		return _len;
	}

	void OnSendPacket(int32 _len) override
	{
		CLInfo("Send Packet. Len={}", _len);
	}
};

NetworkCore::NetworkCore()
{
}

NetworkCore::~NetworkCore()
{
	m_context.stop();
}

void NetworkCore::RunObject()
{	
	auto service = CreateSharedObj<ServerService>(
		NetAddress(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7777)),
		[](tcpSocket _socket, boost::asio::io_context& _context) {
			return CreateSharedObj<GameSession>(std::move(_socket), _context);
		},
		100	// Max Session Count (todo: �����ؾ���)
	);

	service->ServiceStart(m_context);

	for (int32 i = 0; i < 6; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("io thread start. ThreadId={}", LThreadId);
			m_context.run(); 
			});
	}	
}
