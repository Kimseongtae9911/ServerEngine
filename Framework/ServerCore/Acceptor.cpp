#include "pch.h"
#include "Acceptor.h"
#include "SocketUtils.h"
#include "MemoryManager.h"
#include "Session.h"
#include "Service.h"

Acceptor::~Acceptor()
{
	for (auto acceptor : m_acceptorVec)
		acceptor->close();
}

bool Acceptor::StartAccpet(boost::asio::io_context& _context, std::shared_ptr<Service> _service, int32 _count)
{	
	for (int32 i = 0; i < _count; ++i) {
		auto acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(_context);
		SocketUtils::InitAcceptor(acceptor, _service->GetNetAddress());

		acceptor->async_accept([this, &_context, i, _service](boost::system::error_code _err, tcpSocket _socket) {
			if (!_err) {
				SocketUtils::InitializeSocket(_socket, false);
				auto session = _service->CreateSession(_socket, _context);
				session->RunObject();
				ProcessAccept(i, _context, _service, session);
			}
			else {
				//todo: 俊矾贸府
				std::cout << "Error" << std::endl;
				std::cout << _err << std::endl;
				ProcessAccept(i, _context, _service, nullptr);
			}
			});

		m_acceptorVec.push_back(acceptor);
	}

	return true;
}

void Acceptor::ProcessAccept(int32 _index, boost::asio::io_context& _context, std::shared_ptr<Service> _service, SessionRef _session)
{
	_service->AddSession(_session);

	auto acceptor = m_acceptorVec[_index];

	acceptor->async_accept([this, &_context, _index, _service](boost::system::error_code _err, tcpSocket _socket) {
		if (!_err) {
			SocketUtils::InitializeSocket(_socket, false);
			auto session = _service->CreateSession(_socket, _context);
			session->RunObject();
			ProcessAccept(_index, _context, _service, session);
		}
		else {
			//todo: 俊矾贸府
			std::cout << "Error" << std::endl;
			ProcessAccept(_index, _context, _service, nullptr);
		}
		});
}
