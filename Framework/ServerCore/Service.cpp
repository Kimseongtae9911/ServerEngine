#include "pch.h"
#include "Service.h"
#include "Session.h"

Service::Service(ServiceType _type, SessionFactory _factory, int32 _maxSessionCount)
	: m_type(_type), m_sessionFactory(_factory), m_maxSessionCount(_maxSessionCount)
{
}

Service::~Service()
{
}

void Service::CloseService()
{
}

SessionRef Service::CreateSession()
{
	SessionRef session = m_sessionFactory();

	//Register

	return session;
}

void Service::AddSession(SessionRef _session)
{
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
ClientService::ClientService(SessionFactory _factory, int32 _maxSessionCount)
	:Service(ServiceType::Client, _factory, _maxSessionCount)
{
}

bool ClientService::ServiceStart()
{
	return true;
}


/*---------------------
	ServerService
---------------------*/
ServerService::ServerService(SessionFactory _factory, int32 _maxSessionCount)
	: Service(ServiceType::Server, _factory, _maxSessionCount), m_acceptor(m_context)
{
}

bool ServerService::ServiceStart()
{
	if (false == CanStart())
		return false;

	return true;
}

void ServerService::CloseService()
{
	ServerService::CloseService();
}
