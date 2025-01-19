#pragma once
#include <crow.h>
#include "DatabaseManager.h"
#include <map>
#include "Game.h"
#include <mutex>
#include "LobbyManager.h"
class Routes
{
	public:
		Routes() = default;
		Routes(std::shared_ptr<database::PlayerStorage> db, std::shared_ptr<LobbyManager> lobbies) : m_db{ db }, m_lobbies{ lobbies } {}
		void Run(std::shared_ptr<database::PlayerStorage> playerStorage, std::shared_ptr<LobbyManager> lobbies);
	private:
		crow::response LoginPlayer(std::shared_ptr<database::PlayerStorage> playerStorage, const std::string& username);
		crow::response AddPlayerToDatabase(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req);
		crow::response UpdatePlayerFirerate(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username);
		crow::response UpdatePlayerBulletSpeed(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req,const std::string& username);
		crow::response GetPlayersFromDatabase(std::shared_ptr<database::PlayerStorage> playerStorage);
		crow::response FinishGame(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, std::vector<Player>& Players);
		crow::response GetGameStateAsJson(const crow::request& req, const std::string& username);
		crow::response PlayerJoinLobby(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& gameMode, const std::string& username);
		crow::response PlayerMove(const crow::request& req, const std::string& username, const std::string& direction);
		crow::response PlayerFace(const crow::request& req, const std::string& username, const std::string& direction);
		crow::response PlayerShoot(const crow::request& req, const std::string& username);
		crow::response GetPlayers(const crow::request& req, const std::string& username);
		crow::response GetBullets(const crow::request& req, const std::string& username);
		crow::response GameStarted(const crow::request& req, const std::string& username);
		bool IsGameActive(const std::string& username);
private:
		crow::SimpleApp m_app;
		std::shared_ptr<database::PlayerStorage> m_db;
		std::shared_ptr<LobbyManager> m_lobbies;
};

