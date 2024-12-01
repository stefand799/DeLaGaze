#include "Routes.h"

void Routes::Run(database::PlayerStorage& playerStorage)
{
    CROW_ROUTE(m_app,"/players")([&]() {
		return GetPlayersFromDatabase(playerStorage);
    });
    CROW_ROUTE(m_app, "/players/add")([&,this](const crow::request& req) {
		return AddPlayerToDatabase(playerStorage, req);
        });
	CROW_ROUTE(m_app, "/players/update_firerate<int>")([&,this](const crow::request& req,Player& player, int x)
		{
			return UpdatePlayerFirerate(playerStorage, req,player,x);
		});
	CROW_ROUTE(m_app, "/players/update_bullet_speed")([&, this](const crow::request& req, Player& player)
		{
			return UpdatePlayerBulletSpeed(playerStorage, player, req);
		});
}
crow::response Routes::AddPlayerToDatabase(database::PlayerStorage& playerStorage, const crow::request& req) {
    crow::json::rvalue body = crow::json::load(req.body);
    if (!body) {
        return crow::response(400, "Invalid JSON body");
    }
	//de stabilit ce facem in caz ca avem deja un player cu acelasi username
	if (playerStorage.GetPlayer(body["username"].s())) {
		return crow::response(400, "Player already exists");
	}
    Player p;
	p.SetUsername(body["username"].s());
	p.SetScore(0);
	p.SetPoints(0);
	p.SetBulletSpeed(1);
	p.SetBulletSpeedUpgrade(false);
	if (playerStorage.AddPlayer(p)) {
		return crow::response(201, "Player added successfully");
	}
	else {
		return crow::response(500, "Error adding player");
	}
}
crow::response Routes::UpdatePlayerFirerate(database::PlayerStorage& playerStorage, const crow::request& req,Player& player, int x)
{
	player.SetBulletSpeed(x);
	if (playerStorage.UpdatePlayer(player)) {
		return crow::response(200, "Player updated successfully");
	}
	else {
		return crow::response(500, "Error updating player");
	}
}
crow::response Routes::UpdatePlayerBulletSpeed(database::PlayerStorage& playerStorage,Player& player, const crow::request& req)
{
    player.SetBulletSpeedUpgrade(true);
	if (playerStorage.UpdatePlayer(player)) {
		return crow::response(200, "Player updated successfully");
	}
	else {
		return crow::response(500, "Error updating player");
	}
}
crow::response Routes::GetPlayersFromDatabase(database::PlayerStorage& playerStorage) 
{
	std::vector<crow::json::wvalue> players_json;
	for (const auto& player : playerStorage.GetAllPlayers())
	{
		players_json.push_back(crow::json::wvalue{
			{"username", player.GetUsername()},
			{"score", player.GetScore()},
			{"points", player.GetPoints()},
			{"firerate", player.GetBulletSpeed()},
			{"upgrade_bs", player.GetBulletSpeedUpgrade()}
			});
	}
	return crow::json::wvalue{ players_json };
}