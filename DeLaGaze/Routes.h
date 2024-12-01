#pragma once
#include <crow.h>
#include "DatabaseManager.h"
class Routes
{
	public:
		void Run(database::PlayerStorage& playerStorage);
	private:
		crow::response AddPlayerToDatabase(database::PlayerStorage& playerStorage, const crow::request& req);
		crow::response UpdatePlayerFirerate(database::PlayerStorage& playerStorage, const crow::request& req,Player& player, int x);
		crow::response UpdatePlayerBulletSpeed(database::PlayerStorage& playerStorage, Player& player, const crow::request& req);
		crow::response GetPlayersFromDatabase(database::PlayerStorage& playerStorage);
		crow::response FinishGame(database::PlayerStorage& playerStorage, const crow::request& req, std::vector<Player>& Players);
		crow::response JoinGame(database::PlayerStorage& playerStorage, const crow::request& req, Player& Players);
		crow::response PlayerMoveUp(const crow::request& req, Map& m, Player& p);
		crow::response PlayerMoveDown(const crow::request& req, Map& m, Player& p);
		crow::response PlayerMoveLeft(const crow::request& req, Map& m, Player& p);
		crow::response PlayerMoveRight(const crow::request& req, Map& m, Player& p);
		crow::response PlayerShoot(const crow::request& req, Map& m, Player& p);
	private:
		crow::SimpleApp m_app;

};

