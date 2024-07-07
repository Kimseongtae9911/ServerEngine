#pragma once
#include "NetworkInterface.h"

class NetworkCore : public NetworkObject
{
public:
    NetworkCore();
    ~NetworkCore();

    void RunObject() override;

private:
    void StartAccept();

private:
    boost::asio::io_context m_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
};