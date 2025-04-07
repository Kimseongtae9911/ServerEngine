#pragma once


class NetAddress
{
public:
	NetAddress() = default;
	NetAddress(boost::asio::ip::tcp::endpoint _endpoint) : m_endpoint(_endpoint) {}
	NetAddress(std::string _ip, uint16 _port);

	const boost::asio::ip::tcp::endpoint& GetEndPoint() const { return m_endpoint; }
	const std::string& GetIpAddress() { return m_endpoint.address().to_string(); }
	uint16 GetPort() { return m_endpoint.port(); }

public:
	static IN_ADDR Ip2Address(const std::string& _ip);

private:
	boost::asio::ip::tcp::endpoint m_endpoint;
};