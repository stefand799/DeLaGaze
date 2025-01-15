#include "Lobby.h"

Lobby::Lobby(std::string id, GameMode gameMode) : m_id{ id }, m_gameMode{ gameMode }, m_game{ std::make_unique<Game>() }, MAX_PLAYERS{2} {};


bool Lobby::JoinLobby(std::shared_ptr<Player> player) {
    m_players.emplace_back(std::move(player));

    if (m_players.size() == 2 && !m_timerActive) {
        m_timerStart = std::chrono::steady_clock::now();
        m_timerActive = true;
    }

    if (m_players.size() >= MAX_PLAYERS) {
        StartGame();
        return true;
    }

    return true;
}

void Lobby::StartGame() {
    if (m_game) {
        m_game->AddPlayers(m_players);
        m_game->Start();
    }
    std::cout << "Game started in lobby: " << m_id << " with " << m_players.size() << " players." << std::endl;
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