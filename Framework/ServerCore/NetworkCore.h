#pragma once
#include "NetworkInterface.h"
#include "Acceptor.h"

class Session;

class NetworkCore
{
public:
    NetworkCore();
    ~NetworkCore();

    void RunObject();

private:
    boost::asio::io_context m_context;
};