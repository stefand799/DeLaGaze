#include "Lobby.h"

Lobby::Lobby(std::string id, GameMode gameMode, std::shared_ptr < database::PlayerStorage> db) 
    : m_id{ id }, m_gameMode{ gameMode }, m_db{ db }, MAX_PLAYERS{ 4 } {}
bool Lobby::JoinLobby(std::shared_ptr<Player> player) {
    m_players.emplace_back(std::move(player));

    if (m_players.size() == 2 && !m_timerActive) {
        m_timerStart = std::chrono::steady_clock::now();
        m_timerActive = true;

        std::thread([this]() {
            while (m_timerActive) {
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_timerStart).count();

                if (elapsedTime >= TIMER_THRESHOLD) {
                    StartGame();
                    m_timerActive = false;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            }).detach();
    }

    if (m_players.size() >= MAX_PLAYERS) {
        StartGame();
        return true;
    }

    return true;
}



void Lobby::StartGame() {
    m_game = std::make_shared<Game>();
    if (m_game) {
        if (m_game->AddPlayers(m_players,this->GetMode())) {
            std::thread gamethread([&]() {m_game->Start(); });
            gamethread.detach();
            std::cout << "Players added to game";
        }
    }
    std::cout << "Game started in lobby: " << m_id << " with " << m_players.size() << " players." << std::endl;
    if (!m_game->IsRunning()) {
        for (auto& player : m_players)
            m_db->UpdatePlayer(player);
        this->~Lobby();
    }
        
}


// Getters
std::shared_ptr<Game> Lobby::GetGame() {
    if (m_game) return m_game;
    return nullptr;
}
const GameMode& Lobby::GetMode() {
	return m_gameMode;
}
const std::string& Lobby::GetId() {
	return m_id;
}