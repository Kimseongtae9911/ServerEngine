#include "pch.h"
#include "Acceptor.h"
#include "SocketUtils.h"
#include "MemoryManager.h"
#include "Session.h"

Acceptor::~Acceptor()
{
	for (auto acceptor : m_acceptorVec)
		acceptor->close();
}

bool Acceptor::StartAccpet(boost::asio::io_context& _context, const NetAddress& _netAddress, int32 _count)
{	
	for (int32 i = 0; i < _count; ++i) {
		auto acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(_context);
		SocketUtils::InitAcceptor(acceptor, _netAddress);

		acceptor->async_accept([this, &_context, i](boost::system::error_code _err, tcpSocket _socket) {
			if (!_err) {
				SocketUtils::InitializeSocket(_socket, false);

				auto session = std::allocate_shared<Session, StlAllocator<Session>>(m_sessionAllocator, std::move(_socket), std::ref(_context));
				session->RunObject();
				ProcessAccept(i, _context, session);
			}
			else {
				//todo: 俊矾贸府
				std::cout << "Error" << std::endl;
				std::cout << _err << std::endl;
				ProcessAccept(i, _context, nullptr);
			}
			});

		m_acceptorVec.push_back(acceptor);
	}

	return true;
}

void Acceptor::ProcessAccept(int32 _index, boost::asio::io_context& _context, SessionRef _session)
{
	//todo: 技记 殿废

	auto acceptor = m_acceptorVec[_index];

	acceptor->async_accept([this, &_context, _index](boost::system::error_code _err, tcpSocket _socket) {
		if (!_err) {
			auto session = std::allocate_shared<Session, StlAllocator<Session>>(m_sessionAllocator, std::move(_socket), std::ref(_context));
			session->RunObject();
			ProcessAccept(_index, _context, session);
		}
		else {
			//todo: 俊矾贸府
			std::cout << "Error" << std::endl;
			ProcessAccept(_index, _context, nullptr);
		}
		});
}
