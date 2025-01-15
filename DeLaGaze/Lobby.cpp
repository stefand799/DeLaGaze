#include "Lobby.h"

Lobby::Lobby(std::string id, GameMode gameMode) : m_id{ id }, m_gameMode{ gameMode }, m_game{ std::make_unique<Game>() }, MAX_PLAYERS{4} {};


bool Lobby::JoinLobby(std::shared_ptr<Player> player) {
    if (m_players.size() >= MAX_PLAYERS) {
        return false;
    }
    m_players.emplace_back(std::move(player));
    return true;
}


// Getters
std::unique_ptr<Game> Lobby::GetGame() {
	return std::move(m_game);
}
const GameMode& Lobby::GetMode() {
	return m_gameMode;
}
const std::string& Lobby::GetId() {
	return m_id;
}