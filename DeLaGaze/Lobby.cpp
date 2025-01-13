#include "Lobby.h"

Lobby::Lobby()
{
	this->m_game = std::make_shared<Game>();
}

bool Lobby::JoinPlayerInLobby(std::shared_ptr<Player> player)
{
	this->m_players.emplace_back(player);
	return true;
}
