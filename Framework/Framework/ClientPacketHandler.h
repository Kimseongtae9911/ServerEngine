#pragma once
#include "Protocol.pb.h"

extern PacketHandlerFunc GPacketHandler[std::numeric_limits<uint16>::max()];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_C_CHAT = 1004,
	PKT_S_CHAT = 1005,
};

bool Handler_INVALID(PacketSessionRef& _session, uint8* _buffer, int32 _len);
bool Handler_C_LOGIN(PacketSessionRef& _session, Protocol::C_LOGIN&_pkt);
bool Handler_C_ENTER_GAME(PacketSessionRef& _session, Protocol::C_ENTER_GAME&_pkt);
bool Handler_C_CHAT(PacketSessionRef& _session, Protocol::C_CHAT&_pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < std::numeric_limits<uint16>::max(); ++i)
		{
			GPacketHandler[i] = Handler_INVALID;
		}
		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& _session, uint8* _buffer, int32 _len) {return HandlerPacket<Protocol::C_LOGIN>(Handler_C_LOGIN, _session, _buffer, _len); };
		GPacketHandler[PKT_C_ENTER_GAME] = [](PacketSessionRef& _session, uint8* _buffer, int32 _len) {return HandlerPacket<Protocol::C_ENTER_GAME>(Handler_C_ENTER_GAME, _session, _buffer, _len); };
		GPacketHandler[PKT_C_CHAT] = [](PacketSessionRef& _session, uint8* _buffer, int32 _len) {return HandlerPacket<Protocol::C_CHAT>(Handler_C_CHAT, _session, _buffer, _len); };
	}

	static bool HandlePacket(PacketSessionRef& _session, uint8* _buffer, int32 _len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);
		return GPacketHandler[header->protocol](_session, _buffer, _len);
	}
	static SendBufRef MakeSendBuffer(Protocol::S_LOGIN& _pkt) { return MakeSendBuffer(_pkt, PKT_S_LOGIN); }
	static SendBufRef MakeSendBuffer(Protocol::S_ENTER_GAME& _pkt) { return MakeSendBuffer(_pkt, PKT_S_ENTER_GAME); }
	static SendBufRef MakeSendBuffer(Protocol::S_CHAT& _pkt) { return MakeSendBuffer(_pkt, PKT_S_CHAT); }

private:
	template<class PacketType, class ProcessFunc>
	static bool HandlerPacket(ProcessFunc _func, PacketSessionRef& _session, uint8* _buffer, int32 _len)
	{
		PacketType pkt;
		if (false == pkt.ParseFromArray(_buffer + sizeof(PacketHeader), _len - sizeof(PacketHeader)))
			return false;

		GJobQueue->ExecuteAsync(_func(_session, pkt));

		return true;
	}

	template<class T>
	static SendBufRef MakeSendBuffer(T& _pkt, uint16 _pktId)
	{
		const uint16 dataSize = static_cast<uint16>(_pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufRef sendBuffer = GSendBufferPool->UseChunk(packetSize);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->GetBuffer());
		header->size = packetSize;
		header->protocol = _pktId;

		ASSERT_CRASH(_pkt.SerializeToArray(&header[1], dataSize));

		sendBuffer->Close(packetSize);

		return sendBuffer;
	}

};