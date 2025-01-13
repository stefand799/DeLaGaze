#pragma once
#include <crow.h>
#include "DatabaseManager.h"
#include <map>
#include "Game.h"
#include <mutex>
#include "Lobby.h"
class Routes
{
	public:
		Routes() = default;
		Routes(std::shared_ptr<database::PlayerStorage> db, std::shared_ptr<Lobby> lobby) : m_db{ db }, m_lobby{ lobby } {}
		void Run(std::shared_ptr<database::PlayerStorage> playerStorage, std::shared_ptr<Lobby> lobby);
	private:
		crow::response LoginPlayer(std::shared_ptr<database::PlayerStorage> playerStorage, const std::string& username);
		crow::response AddPlayerToDatabase(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req);
		crow::response UpdatePlayerFirerate(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username);
		crow::response UpdatePlayerBulletSpeed(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req,const std::string& username);
		crow::response GetPlayersFromDatabase(std::shared_ptr<database::PlayerStorage> playerStorage);
		crow::response FinishGame(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, std::vector<Player>& Players);
		crow::response PlayerMoveUp(std::shared_ptr<Player>& p, const crow::request& req);
		crow::response PlayerMoveDown(std::shared_ptr<Player>& p, const crow::request& req);
		crow::response PlayerMoveLeft(std::shared_ptr<Player>& p, const crow::request& req);
		crow::response PlayerMoveRight(std::shared_ptr<Player>& p, const crow::request& req);
		crow::response PlayerShoot (std::shared_ptr<Player>& p, const crow::request& req);
		crow::response GetMapAsJson(const crow::request& req);
		crow::response PlayerJoinLobby(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username);
private:
		crow::SimpleApp m_app;
		std::mutex m_mutex;
		std::shared_ptr<database::PlayerStorage> m_db;
		std::shared_ptr<Lobby> m_lobby;
};

