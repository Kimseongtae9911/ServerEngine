#pragma once
#include "NetAddress.h"

class Acceptor
{
public:
	Acceptor() {}
	~Acceptor();

	bool StartAccpet(boost::asio::io_context& _context, const NetAddress& _netAddress, int32 _count = 1);

private:
	void ProcessAccept(int32 index, boost::asio::io_context& _context, SessionRef _session);

protected:	
	StVector<std::shared_ptr<boost::asio::ip::tcp::acceptor>> m_acceptorVec;
	StlAllocator<Session> m_sessionAllocator;
};

