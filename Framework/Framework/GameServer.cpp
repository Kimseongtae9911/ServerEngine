#include "pch.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "NetworkCore.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "Room.h"

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

void DoWorkerJob(ServerServiceRef& _service)
{
    while (true)
    {
        LEndTickCount = ::GetTickCount64() + 64;

        //네트워크 처리 코드 context.run

        // 글로벌 큐
        ThreadManager::DoGlobalQueueWork();

        //context.run을 수행 및 타임아웃이 걸리면 다음 수행
        //GlobalQueueWork는 64만큼 수행?
    }
}

int main()
{
    if (false == Logger::GetInstance()->Initialize())
    {
        ASSERT_CRASH("Logger Initailize Fail");
    }

    CLLog("Logger Initialized");

    ClientPacketHandler::Init();

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