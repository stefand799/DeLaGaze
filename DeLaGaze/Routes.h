#pragma once
#include <crow.h>
#include "DatabaseManager.h"
#include <map>
#include "Game.h"
#include <mutex>
class Routes
{
	public:
		Routes() = default;
		Routes(database::PlayerStorage& db, Game& game) : m_db(db), gameInstance(game) {}
		void Run(database::PlayerStorage& playerStorage);
	private:
		crow::response LoginPlayer(database::PlayerStorage& playerStorage, const std::string& username);
		crow::response AddPlayerToDatabase(database::PlayerStorage& playerStorage, const crow::request& req);
		crow::response UpdatePlayerFirerate(database::PlayerStorage& playerStorage, const crow::request& req,Player& player, int x);
		crow::response UpdatePlayerBulletSpeed(database::PlayerStorage& playerStorage, Player& player, const crow::request& req);
		crow::response GetPlayersFromDatabase(database::PlayerStorage& playerStorage);
		crow::response FinishGame(database::PlayerStorage& playerStorage, const crow::request& req, std::vector<Player>& Players);
		crow::response JoinGame(database::PlayerStorage& playerStorage, const crow::request& req, Player& Players);
		crow::response PlayerMoveUp(Player& p, const crow::request& req);
		crow::response PlayerMoveDown(Player& p, const crow::request& req);
		crow::response PlayerMoveLeft(Player& p, const crow::request& req);
		crow::response PlayerMoveRight(Player& p, const crow::request& req);
		crow::response PlayerShoot (Player& p, const crow::request& req);
		crow::response GetMapAsJson(const crow::request& req);
private:
		crow::SimpleApp m_app;
		std::mutex m_mutex;
		database::PlayerStorage& m_db;
		Game& gameInstance;
};

