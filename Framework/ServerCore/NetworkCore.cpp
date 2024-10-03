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

		auto sendBuffer = CreateSharedObj<SendBuffer>(4096);
		sendBuffer->CopyData(_buffer, _len);

		SendPacket(sendBuffer);

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

void NetworkCore::RunObject(std::shared_ptr<Timer> _timer)
{	
	auto service = CreateSharedObj<ServerService>(
		NetAddress(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7777)),
		[](tcpSocket _socket, boost::asio::io_context& _context) {
			return CreateSharedObj<GameSession>(std::move(_socket), _context);
		},
		100	// Max Session Count (todo: 구현해야함)
	);

	service->ServiceStart(m_context);
	m_timer = std::move(_timer);
	m_timer->SetService(service.get());

	for (int32 i = 0; i < 6; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("io thread start. ThreadId={}", LThreadId);
			m_context.run(); 
			});
	}

	//todo: 전체 세션에 대해 하나의 타이머만 돌리고 있다. 성능 개선이 필요해보임
	for (int32 i = 0; i < 1; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("timer thread start. ThreadId={}", LThreadId);
			m_timer->StartTimer(100);	//timer temp
			});
	}
}
