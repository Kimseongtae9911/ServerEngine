#include "pch.h"
#include "NetworkInterface.h"
#include "Session.h"

Session::Session(tcpSocket _socket) : m_socket(std::move(_socket))
{
	m_recvBuffer.fill(0);
}

Session::~Session()
{
}

void Session::RunObject()
{
	auto self(shared_from_this());
	m_socket.async_read_some(boost::asio::buffer(m_recvBuffer), [this, self](boost::system::error_code _err, std::size_t _length) {
		if (_err == boost::asio::error::eof || _err == boost::asio::error::connection_reset) {
			OnDisconnected();
			return;
		}

		//todo: 패킷 처리
		std::cout << "Recv: " << _length << std::endl;

		RunObject();
		});
}

void Session::OnDisconnected()
{
	std::cout << "Disconnect" << std::endl;
	if (m_socket.is_open()) {
		boost::system::error_code ec;
		m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		m_socket.close(ec);

		if (ec) {
			//todo: 연결해제 중 오류 발생
		}
	}
}
