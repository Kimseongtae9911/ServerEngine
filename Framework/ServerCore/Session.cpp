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
	CLInfo("Session Release");
}

void Session::ProcessRecv()
{
	auto self(shared_from_this());

	std::array<uint8, 1024> recvBuffer;

	m_socket.async_read_some(boost::asio::buffer(recvBuffer), [this, self, recvBuffer](boost::system::error_code _err, std::size_t _length) {
		if (_err == boost::asio::error::eof || _err == boost::asio::error::connection_reset) {
			CLError("Recv Error");
			OnDisconnected();
			return;
		}

		OnRecvPacket(recvBuffer, _length);
		});
}

void Session::SendPacket(uint8* _data, int32 _length)
{
	//todo: sendbuffer 수정
	std::array<uint8, 1024> sendBuffer;
	sendBuffer.fill(0);
	std::memcpy(sendBuffer.data(), _data, _length);

	auto self(shared_from_this());
	boost::asio::async_write(m_socket, boost::asio::buffer(sendBuffer, _length),
		boost::asio::bind_executor(m_strand,
			[this, self](boost::system::error_code ec, std::size_t /*length*/) {
				if (ec) {
					//todo: 에러처리
					OnDisconnected();
					return;
				}
			}));

	OnSendPacket(_length);
}

bool Session::Connect(const NetAddress& _netAddress)
{
	if (m_service->GetServiceType() != ServiceType::Client)
		return false;

	std::vector<boost::asio::ip::tcp::endpoint> endpoints = { _netAddress.GetEndPoint() };

	std::cout << _netAddress.GetEndPoint().address().to_string() << std::endl;

	auto self(shared_from_this());
	boost::asio::async_connect(m_socket, endpoints,
		[this, self](const boost::system::error_code& _error, const boost::asio::ip::tcp::endpoint& _endpoint) {
			if (!_error) {
				ProcessConnected();
			}
			else {
				CLInfo("연결 실패: ErrorCode={}, ErrorMsg={}", _error.value(), _error.message());
				OnDisconnected();
			}
		});
	return true;
}

void Session::DisconnectSession(EResultCode _resultCode)
{
}

void Session::OnSendPacket(int32 _length)
{
	
}

void Session::OnRecvPacket(std::array<uint8, 1024> _buffer, int32 _len)
{	
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
	if (m_isDisconnected.exchange(true))
		return;

	CLInfo("Session Disconnected");

	SocketUtils::CloseSocket(m_socket);
	m_service->ReleaseSession(shared_from_this());
}
