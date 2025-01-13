#pragma once
#include "Game.h"
#include "Player.h"

class Lobby
{
public:
	Lobby();
	bool JoinPlayerInLobby(std::shared_ptr<Player> player);
	std::vector<std::shared_ptr<Player>> m_players;
	std::shared_ptr<Game> m_game;
};

