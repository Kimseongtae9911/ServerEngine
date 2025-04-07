#include "pch.h"
#include "../ServerCore/Network/Service.h"
#include "../ServerCore/Thread/ThreadManager.h"
#include "../ServerCore/Utils/Buffer/BufferReader.h"
#include "ServerPacketHandler.h"

using boost::asio::ip::tcp;

class ServerSession : public PacketSession
{
public:
    ServerSession(tcpSocket _socket, boost::asio::io_context& _context) : PacketSession(std::move(_socket), _context) {}

    ~ServerSession() {
        CLInfo("ServerSession Release");
    }

    void OnConnected() override
    {
        CLInfo("Client{} Connected To Server", m_id);

        m_service->AddSession(shared_from_this());

        Protocol::C_LOGIN pkt;
        auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
        SendPacket(sendBuffer);
    }

    void OnDisconnected() override
    {
        CLInfo("Server Disconnected");
    }

    void ParsePacket(uint8* _buffer, int32 _len) override
    {
        PacketSessionRef session = GetPacketSessionRef();
        PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);

        ServerPacketHandler::HandlePacket(session, _buffer);
    }

    void OnSendPacket(int32 _len) override
    {        
    }
};

int main()
{
    ServerPacketHandler::Init();

    boost::asio::io_context context;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto service = CreateSharedObj<ClientService>(
        NetAddress("127.0.0.1", 7777),
        [](tcpSocket _socket, boost::asio::io_context& _context) {
            return CreateSharedObj<ServerSession>(std::move(_socket), _context);
        },
        100	// Max Session Count (todo: 구현해야함)
    );

    if (false == service->ServiceStart(context))
        return 0;

    for (int32 i = 0; i < 6; ++i) {
        GThreadManager->RunThreads([&context]() {
            CLInfo("io thread start. ThreadId={}", LThreadId);
            context.run();
            });
    }

    Protocol::C_CHAT pkt;
    pkt.set_msg("Hello World!");
    auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);

    while (true)
    {
        service->BroadcastPacket(sendBuffer);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    GThreadManager->JoinThreads();

    return 0;
}