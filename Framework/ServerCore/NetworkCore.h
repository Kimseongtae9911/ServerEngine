#pragma once
#include "NetworkInterface.h"

class NetworkCore
{
public:
    NetworkCore();
    ~NetworkCore();

    void RunObject();

private:
    void StartAccept();

private:
    boost::asio::io_context m_context;
    boost::asio::ip::tcp::acceptor m_acceptor;
};