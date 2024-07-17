#include "pch.h"
#include "NetworkInterface.h"
#include "Session.h"

Session::Session(tcpSocket _socket, boost::asio::io_context& _context) : m_socket(std::move(_socket)), m_strand(_context)
{
	m_recvBuffer.fill(0);
	m_sendBuffer.fill(0);	
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

void Session::SendPacket(int8* _data, int32 _length)
{
	m_sendBuffer.fill(0);
	std::memcpy(m_sendBuffer.data(), _data, _length);
	OnSendPacket(_length);
}

void Session::OnSendPacket(int32 _length)
{
	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(m_sendBuffer, _length),
		boost::asio::bind_executor(m_strand,
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (ec) {
					//todo: 에러처리
					OnDisconnected();
					return;
				}
			}));
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
