#include "pch.h"
#include "ClientPacketHandler.h"
#include "GameSession.h"
#include "Player.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handler_INVALID(PacketSessionRef& _session, uint8* _buffer, int32 _len)
{
	return true;
}

bool Handler_C_LOGIN(PacketSessionRef& _session, Protocol::C_LOGIN& _pkt)
{
	GameSessionRef gameSession = std::static_pointer_cast<GameSession>(_session);

	Protocol::S_LOGIN pkt;
	pkt.set_success(true);

	// todo : id generator
	static Atomic<uint64> idGenerator = 1;

	{
		auto player = pkt.add_players();
		player->set_name("dbName1");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = CreateSharedObj<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->ownerSession = gameSession;

		gameSession->m_players.push_back(playerRef);
	}

	{
		auto player = pkt.add_players();
		player->set_name("dbName2");
		player->set_playertype(Protocol::PLAYER_TYPE_MAGE);

		PlayerRef playerRef = CreateSharedObj<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->ownerSession = gameSession;

		gameSession->m_players.push_back(playerRef);
	}

	auto sendBuuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	gameSession->SendPacket(sendBuuffer);

	return true;
}

bool Handler_C_ENTER_GAME(PacketSessionRef& _session, Protocol::C_ENTER_GAME& _pkt)
{
	GameSessionRef gameSession = std::static_pointer_cast<GameSession>(_session);

	uint64 index = _pkt.playerindex();

	PlayerRef player = gameSession->m_players[index];

	Protocol::S_ENTER_GAME pkt;
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
	player->ownerSession->SendPacket(sendBuffer);

	return true;
}

bool Handler_C_CHAT(PacketSessionRef& _session, Protocol::C_CHAT& _pkt)
{
	CLInfo("{}", _pkt.msg());

	Protocol::S_CHAT pkt;
	pkt.set_msg(_pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);

	return true;
}
