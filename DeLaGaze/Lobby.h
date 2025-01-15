#pragma once
#include "Game.h"
#include "Player.h"
#include "GameMode.h"

class Routes;
class Lobby{
public:
	Lobby(std::string id, GameMode gameMode);
	bool JoinLobby(std::shared_ptr<Player> player);
	friend class Routes;
	std::unique_ptr<Game> GetGame();
	const GameMode& GetMode();
	const std::string& GetId();
private:
	const size_t MAX_PLAYERS;
	std::string m_id;
	GameMode m_gameMode;
	std::vector<std::shared_ptr<Player>> m_players;
	std::unique_ptr<Game> m_game;
};

