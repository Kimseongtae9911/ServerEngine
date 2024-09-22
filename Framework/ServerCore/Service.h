#pragma once
#include "Acceptor.h"

enum class ServiceType : uint8
{
	Server,
	Client
};

class Service : public std::enable_shared_from_this<Service>
{
public:
	using SessionFactory = std::function<SessionRef(tcpSocket, boost::asio::io_context&)>;

	Service(ServiceType _type, const NetAddress& _netAddress, SessionFactory _sessionFactory, int32 _maxSessionCount = 1);
	virtual ~Service();

	virtual bool	ServiceStart(boost::asio::io_context& _context, int32 _count = 1) abstract;
	virtual void	CloseService();

	SessionRef			CreateSession(tcpSocket& _socket, boost::asio::io_context& _context);
	void				AddSession(SessionRef _session);
	void				ReleaseSession(SessionRef _session);
	int32				GetCurrentSessionCount() { return m_sessionCount; }
	int32				GetMaxSessionCount() { return m_maxSessionCount; }
	const NetAddress&	GetNetAddress() const { return m_netAddress; }
	ServiceType			GetServiceType() const { return m_type; }

protected:
	USE_LOCK;

	ServiceType m_type;
	NetAddress m_netAddress;
	SessionFactory m_sessionFactory;

	StSet<SessionRef> m_sessions;
	int32 m_sessionCount = 0;
	int32 m_maxSessionCount = 0;
};

/*---------------------
	ClientService
---------------------*/
class ClientService : public Service
{
public:
	ClientService(const NetAddress& _netAddress, SessionFactory _sessionFactory, int32 _maxSessionCount = 1);

	bool ServiceStart(boost::asio::io_context& _context, int32 _count = 1) override;
};


/*---------------------
	ServerService
---------------------*/
class ServerService : public Service
{
public:
	ServerService(const NetAddress& _netAddress, SessionFactory _sessionFactory, int32 _maxSessionCount = 1);

	bool ServiceStart(boost::asio::io_context& _context, int32 _count = 1) override;
	void CloseService() override;

private:
	AcceptorRef m_acceptor = nullptr;
};