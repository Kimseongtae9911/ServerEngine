#include "pch.h"
#include "ClientPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(PacketSessionRef& _session, uint8* _buffer, int32 _len)
{
	return true;
}

bool Handler_C_TEST(PacketSessionRef& _session, Protocol::C_TEST& _pkt)
{
	return false;
}

bool Handler_C_MOVE(PacketSessionRef& _session, Protocol::C_MOVE& _pkt)
{
	return false;
}
