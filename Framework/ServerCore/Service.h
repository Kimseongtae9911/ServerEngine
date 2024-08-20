#pragma once

enum class ServiceType : uint8
{
	Server,
	Client
};

using SessionFactory = std::function<SessionRef(void)>;

class Service : public std::enable_shared_from_this<Service>
{
public:
	Service(ServiceType _type, SessionFactory _factory, int32 _maxSessionCount = 1);
	virtual ~Service();

	virtual bool	ServiceStart() abstract;
	bool			CanStart() { return m_sessionFactory != nullptr; }
	virtual void	CloseService();
	void			SetSessionFactory(SessionFactory _func) { m_sessionFactory = _func; }

	SessionRef	CreateSession();
	void		AddSession(SessionRef _session);
	void		ReleaseSession(SessionRef _session);
	int32		GetCurrentSessionCount() { return m_sessionCount; }
	int32		GetMaxSessionCount() { return m_maxSessionCount; }

protected:
	USE_LOCK;

	ServiceType m_type;

	StSet<SessionRef> m_sessions;
	int32 m_sessionCount = 0;
	int32 m_maxSessionCount = 0;
	SessionFactory m_sessionFactory;
};

/*---------------------
	ClientService
---------------------*/
class ClientService : public Service
{
public:
	ClientService(SessionFactory _factory, int32 _maxSessionCount = 1);

	bool ServiceStart() override;
};


/*---------------------
	ServerService
---------------------*/
class ServerService : public Service
{
public:
	ServerService(SessionFactory _factory, int32 _maxSessionCount = 1);

	bool ServiceStart() override;
	void CloseService() override;

private:
	boost::asio::io_context m_context;
	boost::asio::ip::tcp::acceptor m_acceptor;
};