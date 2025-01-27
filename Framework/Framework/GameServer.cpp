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

    char sendData[] = "Hello World";
    while (true)
    {
        Protocol::S_TEST pkt;
        pkt.set_id(1000);
        pkt.set_hp(100);
        pkt.set_attack(10);

        {
            Protocol::BuffData* data = pkt.add_buffs();
            data->set_buffid(100);
            data->set_remaintime(1.2f);
            data->add_victims(4000);
        }

        {
            Protocol::BuffData* data = pkt.add_buffs();
            data->set_buffid(200);
            data->set_remaintime(2.5f);
            data->add_victims(1000);
            data->add_victims(2000);
        }

        SendBufRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);

        GameSessionMgr->Broadcast(sendBuffer);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
    

    GThreadManager->JoinThreads();    
}