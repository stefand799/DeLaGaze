#include "LobbyManager.h"

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

//std::shared_ptr<Lobby> LobbyManager::CreateLobby() {
//	auto id = GenerateUniqueId();
//	auto lobby = std::make_shared<Lobby>(id);
//	m_lobbies[id] = lobby;
//	return lobby;
//}
//
//std::shared_ptr<Lobby> LobbyManager::JoinALobby(std::shared_ptr<Player> player) {
//	std::lock_guard<std::mutex> lock(m_mutex);
//	// Aici se intra in lobby deci verificare deci pot verifica scorul
//	std::string playerUsername = player->GetUsername();
//	for (auto& [id, lobby] : m_lobbies) {
//		lobby->JoinLobby(std::move(player));
//		m_playerToLobby[playerUsername] = lobby;
//		return lobby;
//	}
//	auto newLobby = CreateLobby();
//	newLobby->JoinLobby(std::move(player));
//	m_playerToLobby[playerUsername] = newLobby;
//	return newLobby;
//}