#include "pch.h"
#include "Session.h"
#include "Service.h"
#include "SocketUtils.h"

Session::Session(tcpSocket _socket, boost::asio::io_context& _context) : m_socket(std::move(_socket)), m_strand(_context)
{
	m_recvBuffer.fill(0);
	m_sendBuffer.fill(0);	
}

Session::~Session()
{
	LInfo("Session Release");
}

void Session::ProcessRecv()
{
	auto self(shared_from_this());

	std::array<uint8, 1024> recvBuffer;

	m_socket.async_read_some(boost::asio::buffer(recvBuffer), [this, self, recvBuffer](boost::system::error_code _err, std::size_t _length) {
		if (_err == boost::asio::error::eof || _err == boost::asio::error::connection_reset) {
			OnDisconnected();
			return;
		}

		OnRecvPacket(recvBuffer, _length);
		});
}

void Session::SendPacket(int8* _data, int32 _length)
{
	m_sendBuffer.fill(0);
	std::memcpy(m_sendBuffer.data(), _data, _length);
	OnSendPacket(_length);
}

void Session::DisconnectSession(EResultCode _resultCode)
{
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

void Session::OnRecvPacket(std::array<uint8, 1024> _buffer, int32 _len)
{
	CLInfo("Recv Packet. Len={}", _len);
	ProcessRecv();
}

void Session::ProcessConnected()
{
	OnConnected();

	ProcessRecv();
}

void Session::OnConnected()
{
}

void Session::OnDisconnected()
{
	CLInfo("Session Disconnected");
	m_service->ReleaseSession(shared_from_this());

	SocketUtils::CloseSocket(m_socket);

	//todo: 세션 메모리 해제가 안됨..
}
