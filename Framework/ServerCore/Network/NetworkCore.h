#pragma once
#include "Timer/Timer.h"

class NetworkCore
{
public:
    NetworkCore();
    ~NetworkCore();

    void RunObject(std::shared_ptr<Service> _service, std::shared_ptr<Timer> _timer);

private:
    boost::asio::io_context m_context;
    std::shared_ptr<Timer> m_timer;
};