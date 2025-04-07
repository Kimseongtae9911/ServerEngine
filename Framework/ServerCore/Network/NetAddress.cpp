#include "../pch.h"

NetAddress::NetAddress(std::string _ip, uint16 _port)
{
	boost::asio::ip::address ip_address = boost::asio::ip::make_address(_ip);
	m_endpoint = boost::asio::ip::tcp::endpoint(ip_address, _port);
}

IN_ADDR NetAddress::Ip2Address(const std::string& _ip)
{
    IN_ADDR addr;

#ifdef _WINDOW
	WCHAR w_ip[46]; // IPv6±îÁö °í·ÁÇÑ ³Ë³ËÇÑ Å©±â
	MultiByteToWideChar(CP_UTF8, 0, _ip.c_str(), -1, w_ip, 46);
	if (InetPton(AF_INET, w_ip, &addr) != 1)
#else
	if (inet_pton(AF_INET, _ip.c_str(), &addr) != 1)
#endif
	{
		//todo:: Error
	}


    return addr;
}
