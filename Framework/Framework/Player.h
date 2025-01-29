#pragma once
class Player
{
public:
	uint64 playerId = 0;
	std::string name = "";
	Protocol::PlayerType type = Protocol::PLAYER_TYPE_NONE;
	GameSessionRef ownerSession;
};

