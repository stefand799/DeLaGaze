#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "Lobby.h"
#include "DatabaseManager.h"

class Routes;
class LobbyManager
{
public:
	LobbyManager(std::shared_ptr<database::PlayerStorage> db);
	std::shared_ptr<Lobby> JoinALobby(std::shared_ptr<Player> player, GameMode gameMode);
	std::shared_ptr<Lobby> GetLobbyByPlayer(const std::string& playerUsername);
private:
	std::shared_ptr<database::PlayerStorage> m_db;
	std::unordered_map<std::string, std::shared_ptr<Lobby>> m_lobbies;
	std::unordered_map<std::string, std::shared_ptr<Lobby>> m_playerToLobby;

	std::shared_ptr<Lobby> CreateLobby(GameMode gameMode);
	std::string GenerateUniqueId();
};

