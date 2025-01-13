#include "Lobby.h"

Lobby::Lobby()
{
	this->m_game = std::make_unique<Game>();
}

bool Lobby::JoinPlayerInLobby(std::shared_ptr<Player> player)
{
	this->m_players.emplace_back(std::move(player));
	return true;
}
