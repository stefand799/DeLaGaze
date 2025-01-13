#pragma once
#include "Game.h"
#include "Player.h"

class Lobby
{
public:
	Lobby();
private:
	std::unique_ptr<Game> m_game;
	std::vector<std::shared_ptr<Player>> m_players;
};

