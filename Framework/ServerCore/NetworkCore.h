#pragma once

class NetworkCore
{
public:
    NetworkCore();
    ~NetworkCore();

    void RunObject();

private:
    boost::asio::io_context m_context;
};