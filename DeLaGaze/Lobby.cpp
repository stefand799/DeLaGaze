#include "Lobby.h"

Lobby::Lobby()
{
	this->m_game = std::make_unique<Game>();
}

void Lobby::JoinPlayerInLobby(std::shared_ptr<Player>& player)
{

}
