#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "ThreadManager.h"

using boost::asio::ip::tcp;

class ServerSession : public Session
{
public:
    ServerSession(tcpSocket _socket, boost::asio::io_context& _context) : Session(std::move(_socket), _context) {}

    ~ServerSession() {
        CLInfo("ServerSession Release");
    }

    void OnConnected() override
    {
        std::string str = "Hello World";
        CLInfo("Client{} Connected To Server", m_id);

        auto sendBuffer = GSendBufferPool->UseChunk(4096);
        ::memcpy_s(sendBuffer->GetBuffer(), str.size(), str.data(), str.size());
        sendBuffer->Close(str.size());

        SendPacket(sendBuffer);
    }

    void OnDisconnected() override
    {
        CLInfo("Server Disconnected");
    }

    int32 ProcessPacket(uint8* _buffer, int32 _len) override
    {
        CLInfo("Dummy Client{} Recv Packet. Len={}", m_id, _len);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto sendBuffer = GSendBufferPool->UseChunk(4096);
        ::memcpy_s(sendBuffer->GetBuffer(), _len, _buffer, _len);
        sendBuffer->Close(_len);

        SendPacket(sendBuffer);

        return _len;
    }

    void OnSendPacket(int32 _len) override
    {
        CLInfo("Dummy Client{} Send Packet. Len={}", m_id, _len);
    }
};

int main()
{
    boost::asio::io_context context;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto service = CreateSharedObj<ClientService>(
        NetAddress("127.0.0.1", 7777),
        [](tcpSocket _socket, boost::asio::io_context& _context) {
            return CreateSharedObj<ServerSession>(std::move(_socket), _context);
        },
        1	// Max Session Count (todo: 구현해야함)
    );

    if (false == service->ServiceStart(context))
        return 0;

    for (int32 i = 0; i < 6; ++i) {
        GThreadManager->RunThreads([&context]() {
            CLInfo("io thread start. ThreadId={}", LThreadId);
            context.run();
            });
    }

    GThreadManager->JoinThreads();

    return 0;
}