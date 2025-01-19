#pragma once
#include "Game.h"
#include "Player.h"
#include "GameMode.h"
#include "DatabaseManager.h"

class Routes;
class Lobby{
public:
	Lobby(std::string id, GameMode gameMode, std::shared_ptr<database::PlayerStorage> db);
	bool JoinLobby(std::shared_ptr<Player> player);
	void StartGame();
	friend class Routes;
	std::shared_ptr<Game> GetGame();
	const GameMode& GetMode();
	const std::string& GetId();
private:
	std::shared_ptr<database::PlayerStorage> m_db;
	std::chrono::time_point<std::chrono::steady_clock> m_timerStart;
	bool m_timerActive = false;
	const int MAX_PLAYERS = 4;
	const int TIMER_THRESHOLD = 10;
	std::string m_id;
	GameMode m_gameMode;
	std::vector<std::shared_ptr<Player>> m_players;
	std::shared_ptr<Game> m_game;
};

