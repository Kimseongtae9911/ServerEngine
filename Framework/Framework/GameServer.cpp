#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"
#include "Session.h"
#include "Service.h"
#include "GameSession.h"

class GameTimer : public Timer
{
public:
    void OnTimer() override
    {
        if (nullptr == m_service)
        {
            StopTimer();
            return;
        }

        for (auto session : m_service->GetAllSession())
            session->OnTimer();
    }
};

int main()
{
    if (false == Logger::GetInstance()->Initialize())
    {
        ASSERT_CRASH("Logger Initailize Fail");
    }

    CLLog("Logger Initialized");

    auto service = CreateSharedObj<ServerService>(
        NetAddress(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 7777)),
        [](tcpSocket _socket, boost::asio::io_context& _context) {
            return CreateSharedObj<GameSession>(std::move(_socket), _context);
        },
        100	// Max Session Count (todo: 구현해야함)
    );

    NetworkCore server;
    server.RunObject(service, CreateSharedObj<GameTimer>());

    GThreadManager->JoinThreads();    
}