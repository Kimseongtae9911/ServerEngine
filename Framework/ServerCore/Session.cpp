#include "pch.h"
#include "Session.h"
#include "Service.h"
#include "SocketUtils.h"

Session::Session(tcpSocket _socket, boost::asio::io_context& _context) : m_socket(std::move(_socket)), m_strand(_context), m_recvBuffer(BUFFER_SIZE)
{
}

Session::~Session()
{
	CLInfo("Session Release");
}

void Session::ProcessRecv()
{
	auto self(shared_from_this());

	m_socket.async_read_some(boost::asio::buffer(m_recvBuffer.GetWritePos(), m_recvBuffer.GetFreeSize()), 
		[this, self](boost::system::error_code _err, std::size_t _length) {
		if (_err == boost::asio::error::eof || _err == boost::asio::error::connection_reset) {
			CLError("Recv Error, ErrCode={}, ErrMsg={}", _err.value(), _err.message());
			OnDisconnected();
			return;
		}

		OnRecvPacket(_length);
		});
}

void Session::SendPacket(SendBufRef _sendBuffer, bool _immediately)
{
	if (_immediately) {
		auto self(shared_from_this());
		boost::asio::async_write(m_socket,
			boost::asio::buffer(_sendBuffer->GetBuffer(), _sendBuffer->GetWriteSize()),
			boost::asio::bind_executor(m_strand,
				[this, self, _sendBuffer](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (ec)
					{
						OnDisconnected();
						return;
					}
					OnSendPacket(_sendBuffer->GetWriteSize());
				}));
		FlushSendQueue();
	}
	else {
		m_sendQueue.push(_sendBuffer);
	}
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
				CLInfo("Connect Error: ErrorCode={}, ErrorMsg={}", _error.value(), _error.message());
				OnDisconnected();
			}
		});
	return true;
}

void Session::DisconnectSession(EResultCode _resultCode)
{
	OnDisconnected();
}

void Session::OnSendPacket(int32 _length)
{
	
}

void Session::OnRecvPacket(int32 _len)
{	
	if (_len == 0) {
		CLError("Recv Zero Bytes");
		DisconnectSession(EResultCode::Disconnect);
		return;
	}
	
	if (false == m_recvBuffer.OnWrite(_len)) {
		CLError("BufferOverflow");
		DisconnectSession(EResultCode::BufferOverflow);
		return;
	}

	int32 recvLen = m_recvBuffer.GetDataSize();
	int32 processLen = ProcessPacket(m_recvBuffer.GetReadPos(), recvLen);
	if (processLen < 0 || processLen > recvLen || false == m_recvBuffer.OnRead(processLen)) {
		DisconnectSession(EResultCode::BufferOverflow);
		return;
	}

	m_recvBuffer.ClearBuffer();

	ProcessRecv();
}

int32 Session::ProcessPacket(uint8* _buffer, int32 _len)
{
	return int32();
}

void Session::FlushSendQueue()
{
	if (m_sendQueue.empty())
		return;

	
	while (true) {
		SendBufRef buffer;
		if (false == m_sendQueue.try_pop(buffer))
			break;

		auto self(shared_from_this());
		boost::asio::async_write(m_socket,
			boost::asio::buffer(buffer->GetBuffer(), buffer->GetWriteSize()),
			boost::asio::bind_executor(m_strand,
				[this, self, buffer](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (ec)
					{
						OnDisconnected();
						return;
					}
					OnSendPacket(buffer->GetWriteSize());
				}));
	}
}

void Session::OnTimer()
{
	//Check SendPacket Queue
	FlushSendQueue();
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
