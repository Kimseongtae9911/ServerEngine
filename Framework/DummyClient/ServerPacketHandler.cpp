#include "pch.h"
#include "ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[std::numeric_limits<uint16>::max()];

bool Handler_INVALID(PacketSessionRef& _session, uint8* _buffer, int32 _len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);

    return true;
}

bool Handler_S_LOGIN(PacketSessionRef& _session, Protocol::S_LOGIN& _pkt)
{
    if (_pkt.success() == false)
        return false;

    if(_pkt.players().size() == 0)
    {

    }

    Protocol::C_ENTER_GAME pkt;
    pkt.set_playerindex(0);
    auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
    _session->SendPacket(sendBuffer);

    return true;
}

bool Handler_S_ENTER_GAME(PacketSessionRef& _session, Protocol::S_ENTER_GAME& _pkt)
{
    return true;
}

bool Handler_S_CHAT(PacketSessionRef& _session, Protocol::S_CHAT& _pkt)
{
    CLInfo("{}", _pkt.msg());

    return true;
}
