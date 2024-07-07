#include "pch.h"
#include "NetworkCore.h"
#include "Session.h"
#include "SocketUtils.h"

NetworkCore::NetworkCore() : m_acceptor(m_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7777))
{
	//todo: 소켓 옵션, 포트 등 설정필요
	SocketUtils::InitializeSocket(m_acceptor);
}

NetworkCore::~NetworkCore()
{
	m_context.stop();
	m_acceptor.close();
}

void NetworkCore::RunObject()
{
	StartAccept();
	m_context.run();
}

void NetworkCore::StartAccept()
{
	m_acceptor.async_accept([this](boost::system::error_code _err, tcpSocket _socket) {
		if (!_err) {
			std::make_shared<Session>(std::move(_socket))->RunObject();
		}

		StartAccept();
		});
}
