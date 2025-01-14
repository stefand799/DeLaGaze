#include "Lobby.h"

Lobby::Lobby(std::string id) : m_id{ id }, m_game{ std::make_unique<Game>()} {};


bool Lobby::JoinLobby(std::shared_ptr<Player> player)
{
	this->m_players.emplace_back(std::move(player));
	return true;
}

std::unique_ptr<Game> Lobby::GetGame() {
	return std::move(m_game);
}