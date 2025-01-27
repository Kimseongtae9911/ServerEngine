#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, uint8*, int32)>;
extern PacketHandlerFunc GPacketHandler[std::numeric_limits<uint16>::max()];

enum : uint16
{
	PKT_C_TEST = 1000,
	PKT_C_MOVE = 1001,
	PKT_S_TEST = 1002,
	PKT_S_LOGIN = 1003,
};

bool Handler_INVALID(PacketSessionRef& _session, uint8* _buffer, int32 _len);
bool Handler_C_TEST(PacketSessionRef& _session, Protocol::C_TEST&_pkt);
bool Handler_C_MOVE(PacketSessionRef& _session, Protocol::C_MOVE&_pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < std::numeric_limits<uint16>::max(); ++i)
		{
			GPacketHandler[i] = Handler_INVALID;
		}
		GPacketHandler[PKT_C_TEST] = [](PacketSessionRef& _session, uint8* _buffer, int32 _len) {return HandlerPacket<Protocol::C_TEST>(Handler_C_TEST, _session, _buffer, _len); };
		GPacketHandler[PKT_C_MOVE] = [](PacketSessionRef& _session, uint8* _buffer, int32 _len) {return HandlerPacket<Protocol::C_MOVE>(Handler_C_MOVE, _session, _buffer, _len); };
	}

	static bool HandlePacket(PacketSessionRef& _session, uint8* _buffer, int32 _len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);
		return GPacketHandler[header->protocol](_session, _buffer, _len);
	}
	static SendBufRef MakeSendBuffer(Protocol::S_TEST& _pkt) { return MakeSendBuffer(_pkt, PKT_S_TEST); }
	static SendBufRef MakeSendBuffer(Protocol::S_LOGIN& _pkt) { return MakeSendBuffer(_pkt, PKT_S_LOGIN); }

private:
	template<class PacketType, class ProcessFunc>
	static bool HandlerPacket(ProcessFunc _func, PacketSessionRef& _session, uint8* _buffer, int32 _len)
	{
		PacketType pkt;
		if (false == pkt.ParseFromArray(_buffer + sizeof(PacketHeader), _len - sizeof(PacketHeader)))
			return false;

		return _func(_session, pkt);
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