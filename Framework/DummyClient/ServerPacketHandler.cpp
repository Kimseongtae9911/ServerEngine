#include "pch.h"
#include "ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[std::numeric_limits<uint16>::max()];

bool Handler_INVALID(PacketSessionRef& _session, uint8* _buffer, int32 _len)
{
    PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);

    return true;
}

bool Handler_S_TEST(PacketSessionRef& _session, Protocol::S_TEST& _pkt)
{
    std::cout << _pkt.id();

    CLLog("{}, {}, {}", _pkt.id(), _pkt.hp(), _pkt.attack());
    CLLog("Bufsize:", _pkt.buffs_size());

    for (auto& buf : _pkt.buffs())
    {
        CLLog("BUFINFO : {}, {}", buf.buffid(), buf.remaintime());
        CLLog("VICTIMS : {}", buf.victims_size());

        for (auto& vic : buf.victims())
        {
            CLLog("{}", vic);
        }
    }

    return true;
}
