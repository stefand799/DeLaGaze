#include "Lobby.h"

Lobby::Lobby() {};
Lobby::Lobby(std::string id) : m_id{ id }, m_game{ nullptr }{};


bool Lobby::JoinLobby(std::shared_ptr<Player> player)
{
	this->m_players.emplace_back(std::move(player));
	return true;
}
