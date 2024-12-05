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
		crow::response PlayerMoveUp(Player& p, const crow::request& req);
		crow::response PlayerMoveDown(Player& p, const crow::request& req);
		crow::response PlayerMoveLeft(Player& p, const crow::request& req);
		crow::response PlayerMoveRight(Player& p, const crow::request& req);
		crow::response PlayerShoot (Player& p, const crow::request& req);
		crow::response GetBulletsFromPlayer(Player& p, const crow::request& req);
	private:
		crow::SimpleApp m_app;

};

