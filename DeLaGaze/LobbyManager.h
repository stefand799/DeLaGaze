#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include "Lobby.h"

class Routes;
class LobbyManager
{
public:
	std::shared_ptr<Lobby> JoinALobby(std::shared_ptr<Player> player, GameMode gameMode);
	std::shared_ptr<Lobby> GetLobbyByPlayer(const std::string& playerUsername);
private:
	std::unordered_map<std::string, std::shared_ptr<Lobby>> m_lobbies;
	std::unordered_map<std::string, std::shared_ptr<Lobby>> m_playerToLobby;
	std::mutex m_mutex;

	std::shared_ptr<Lobby> CreateLobby(GameMode gameMode);
	std::string GenerateUniqueId();
};

