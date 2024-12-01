#pragma once
#include <crow.h>
#include "DatabaseManager.h"
class Routes
{
	public:
		Routes(crow::SimpleApp& app);
		void Run(database::PlayerStorage& playerStorage);
	private:
		crow::response AddPlayerToDatabase(database::PlayerStorage& playerStorage, const crow::request& req, const Player& Player);
		crow::response UpdatePlayerFirerate(database::PlayerStorage& playerStorage, const crow::request& req, Player& Player);
		crow::response UpdatePlayerBulletSpeed(database::PlayerStorage& playerStorage, const crow::request& req, Player& Player);
		crow::response GetPlayersFromDatabase(database::PlayerStorage& playerStorage);
		crow::response FinishGame(database::PlayerStorage& playerStorage, const crow::request& req, std::vector<Player>& Players);
		crow::response JoinGame(database::PlayerStorage& playerStorage, const crow::request& req, Player& Players);

	private:
		crow::SimpleApp m_app;
		database::PlayerStorage m_playerStorage;

};

