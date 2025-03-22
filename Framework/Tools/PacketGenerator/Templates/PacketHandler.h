#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(GameSessionRef&, uint8*, int32)>;
extern PacketHandlerFunc GPacketHandler[std::numeric_limits<uint16>::max()];

enum : uint16
{
{%- for pkt in parser.total_pkt %}
	PKT_{{pkt.name}} = {{pkt.id}},
{%- endfor %}
};

bool Handler_INVALID(GameSessionRef& _session, uint8* _buffer, int32 _len);

{%- for pkt in parser.recv_pkt %}
bool Handler_{{pkt.name}}(GameSessionRef& _session, Protocol::{{pkt.name}}&_pkt);
{%- endfor %}

class {{output}}
{
public:
	static void Init()
	{
		for (int32 i = 0; i < std::numeric_limits<uint16>::max(); ++i)
		{
			GPacketHandler[i] = Handler_INVALID;
		}

{%- for pkt in parser.recv_pkt %}
		GPacketHandler[PKT_{{pkt.name}}] = [](GameSessionRef& _session, uint8* _buffer, int32 _len) {return HandlerPacket<Protocol::{{pkt.name}}>(Handler_{{pkt.name}}, _session, _buffer, _len); };
{%- endfor %}
	}

	static bool HandlePacket(GameSessionRef& _session, uint8* _buffer, int32 _len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(_buffer);
		return GPacketHandler[header->protocol](_session, _buffer, _len);
	}

{%- for pkt in parser.send_pkt %}
	static SendBufRef MakeSendBuffer(Protocol::{{pkt.name}}& _pkt) { return MakeSendBuffer(_pkt, PKT_{{pkt.name}}); }
{%- endfor %}

private:
	template<class PacketType, class ProcessFunc>
	static bool HandlerPacket(ProcessFunc _func, GameSessionRef& _session, uint8* _buffer, int32 _len)
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
