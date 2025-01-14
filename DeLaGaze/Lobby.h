#pragma once
#include "Game.h"
#include "Player.h"

class Routes;
class Lobby
{
public:
	Lobby();
	Lobby(std::string id);
	bool JoinLobby(std::shared_ptr<Player> player);
	friend class Routes;
private:
	std::string m_id;
	std::vector<std::shared_ptr<Player>> m_players;
	std::shared_ptr<Game> m_game;
};

