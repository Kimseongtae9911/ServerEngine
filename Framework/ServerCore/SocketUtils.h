#pragma once

class SocketUtils
{	
public:
	template<typename T>
	static bool InitializeSocket(T& _socket, bool _lingerEnable = true, int32 _seconds = 10, bool _reuseEnable = true, int32 _recvSize = 65000, int32 _sendSize = 65000);

	template<typename T>
	static bool SetLinger(T& _socket, bool _enable, int32 _seconds);

	template<typename T>
	static bool SetReuseAddress(T& _socket, bool _enable);

	template<typename T>
	static bool SetReceiveBufferSize(T& _socket, int32 _size);

	template<typename T>
	static bool SetSendBufferSize(T& _socket, int32 _size);
};

template<typename T>
inline bool SocketUtils::InitializeSocket(T& _socket, bool _lingerEnable, int32 _seconds, bool _reuseEnable, int32 _recvSize, int32 _sendSize)
{
	try {
		SetLinger(_socket, _lingerEnable, _seconds);
		SetReuseAddress(_socket, _reuseEnable);
		SetReceiveBufferSize(_socket, _recvSize);
		SetSendBufferSize(_socket, _sendSize);
	}
	catch (std::exception& e) {
		//todo: 로그 추가
		return false;
	}
}

template<typename T>
inline bool SocketUtils::SetLinger(T& _socket, bool _enable, int32 _seconds)
{
	try {
		boost::asio::socket_base::linger option(_enable, _seconds);
		_socket.set_option(option);
		return true;
	}
	catch (const std::exception& e) {
		//todo: 로그 추가
		return false;
	}
}

template<typename T>
inline bool SocketUtils::SetReuseAddress(T& _socket, bool _enable)
{
	try {
		boost::asio::socket_base::reuse_address option(_enable);
		_socket.set_option(option);
		return true;
	}
	catch (const std::exception& e) {
		//todo: 로그 추가
		return false;
	}
}

template<typename T>
inline bool SocketUtils::SetReceiveBufferSize(T& _socket, int32 _size)
{
	try
	{
		boost::asio::socket_base::receive_buffer_size option(_size);
		_socket.set_option(option);
		return true;
	}
	catch (std::exception& e)
	{
		//todo: 로그 추가
		return false;
	}
}

template<typename T>
inline bool SocketUtils::SetSendBufferSize(T& _socket, int32 _size)
{
	try
	{
		boost::asio::socket_base::send_buffer_size option(_size);
		_socket.set_option(option);
		return true;
	}
	catch (std::exception& e)
	{
		//todo: 로그 추가
		return false;
	}
}
