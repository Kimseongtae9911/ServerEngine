#include "pch.h"
#include "NetworkCore.h"
#include "Session.h"
#include "SocketUtils.h"
#include "ThreadManager.h"

NetworkCore::NetworkCore()
{
}

NetworkCore::~NetworkCore()
{
	m_context.stop();
}

void NetworkCore::RunObject()
{	
	m_acceptor.StartAccpet(m_context, NetAddress(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7777)));


	for (int32 i = 0; i < 6; ++i) {
		GThreadManager->RunThreads([this]() {
			m_context.run(); 
			});
	}	
}
