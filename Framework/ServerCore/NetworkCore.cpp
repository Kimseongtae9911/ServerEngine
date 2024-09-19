#include "pch.h"
#include "NetworkCore.h"
#include "Session.h"
#include "SocketUtils.h"
#include "ThreadManager.h"
#include "Service.h"

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
			return CreateSharedObj<Session>(std::move(_socket), _context);
		},
		100	// Max Session Count (todo: 구현해야함)
	);

	service->ServiceStart(m_context);

	for (int32 i = 0; i < 6; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("io thread start. ThreadId={}", LThreadId);
			m_context.run(); 
			});
	}	
}
