#include "LobbyManager.h"

LobbyManager::LobbyManager(std::shared_ptr<database::PlayerStorage> db) : m_db{ db } {};

std::string LobbyManager::GenerateUniqueId() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dis(0, 15);

	std::stringstream ss;
	for (int i = 0; i < 8; ++i) {
		ss << std::hex << dis(gen);
	}

	return ss.str();
}

std::shared_ptr<Lobby> LobbyManager::CreateLobby(GameMode gameMode) {
	auto id = GenerateUniqueId();
	auto lobby = std::make_shared<Lobby>(id, gameMode, m_db);
	m_lobbies[id] = lobby;
	return lobby;
}


std::shared_ptr<Lobby> LobbyManager::JoinALobby(std::shared_ptr<Player> player, GameMode gameMode) {
    std::string playerUsername = player->GetUsername();

    for (auto& [id, lobby] : m_lobbies) {
        if (lobby->GetMode() == gameMode && lobby->GetGame() == nullptr) {
            lobby->JoinLobby(player);
            m_playerToLobby[playerUsername] = lobby;
            return lobby;
        }
    }

    auto newLobby = CreateLobby(gameMode);
    newLobby->JoinLobby(player);
    m_playerToLobby[playerUsername] = newLobby;
    return newLobby;
}


std::shared_ptr<Lobby> LobbyManager::GetLobbyByPlayer(const std::string& playerUsername) {

	return m_playerToLobby[playerUsername];
}