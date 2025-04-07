#include "../pch.h"

bool SocketUtils::InitAcceptor(std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor, const NetAddress& _netAddress)
{
	boost::system::error_code ec;
	boost::asio::ip::tcp::endpoint endpoint = _netAddress.GetEndPoint();
	_acceptor->open(endpoint.protocol(), ec);
	_acceptor->set_option(boost::asio::socket_base::reuse_address(true));
	_acceptor->bind(endpoint);
	_acceptor->listen();

	return true;
}
