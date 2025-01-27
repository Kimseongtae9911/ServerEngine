#include "pch.h"
#include "Service.h"

Service::Service(ServiceType _type, const NetAddress& _netAddress, SessionFactory _sessionFactory, int32 _maxSessionCount)
	: m_type(_type), m_netAddress(_netAddress), m_sessionFactory(_sessionFactory), m_maxSessionCount(_maxSessionCount)
{
}

Service::~Service()
{
}

void Service::CloseService()
{
}

SessionRef Service::CreateSession(tcpSocket& _socket, boost::asio::io_context& _context)
{
	return m_sessionFactory(std::move(_socket), _context);
}

void Service::AddSession(SessionRef _session)
{
	_session->SetService(shared_from_this().get());

	WRITE_LOCK;
	++m_sessionCount;
	m_sessions.insert(_session);
}

void Service::ReleaseSession(SessionRef _session)
{
	WRITE_LOCK;
	ASSERT_CRASH(m_sessions.erase(_session) != 0);
	--m_sessionCount;
}


/*---------------------
	ClientService
---------------------*/
ClientService::ClientService(const NetAddress& _netAddress, SessionFactory _sessionFactory, int32 _maxSessionCount)
	:Service(ServiceType::Client, _netAddress, _sessionFactory, _maxSessionCount)
{
}

bool ClientService::ServiceStart(boost::asio::io_context& _context, int32 _count)
{
	for (int32 i = 0; i < GetMaxSessionCount(); ++i) {
		tcpSocket socket(_context);
		auto session = CreateSession(socket, _context);
		session->SetService(this);
		session->SetId(i);
		if (false == session->Connect(m_netAddress))
			return false;
	}

	return true;
}


/*---------------------
	ServerService
---------------------*/
ServerService::ServerService(const NetAddress& _netAddress, SessionFactory _sessionFactory, int32 _maxSessionCount)
	: Service(ServiceType::Server, _netAddress, _sessionFactory, _maxSessionCount)
{
}

bool ServerService::ServiceStart(boost::asio::io_context& _context, int32 _count)
{
	m_acceptor = CreateSharedObj<Acceptor>();

	m_acceptor->StartAccpet(_context, shared_from_this(), _count);

	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}
