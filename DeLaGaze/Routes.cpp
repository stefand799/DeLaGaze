#include "Routes.h"

void Routes::Run(database::PlayerStorage& playerStorage)
{
	CROW_ROUTE(m_app, "/login/<string>")([&, this](const crow::request& req, const std::string& username) {
		return LoginPlayer(playerStorage, username);
		});

    CROW_ROUTE(m_app,"/players")([&]() {
		return GetPlayersFromDatabase(playerStorage);
    });
    CROW_ROUTE(m_app, "/players/add")([&,this](const crow::request& req) {
		return AddPlayerToDatabase(playerStorage, req);
        });
	CROW_ROUTE(m_app, "/players/update_firerate/<string>/<int>")([&,this](const crow::request& req, const std::string& username, const int& x)
		{
			Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), req.url_params.get("username"));
			return UpdatePlayerFirerate(playerStorage, req,*player,x);
		});
	CROW_ROUTE(m_app, "/players/update_bullet_speed/<string>")([&, this](const crow::request& req, const std::string& username)
		{
			Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), req.url_params.get("username"));
			return UpdatePlayerBulletSpeed(playerStorage, *player, req);
		});
	CROW_ROUTE(m_app, "/player/move_up")([&, this](const crow::request& req)
		{
			Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), req.url_params.get("username"));
			return PlayerMoveUp(*player, req);
		});
	CROW_ROUTE(m_app, "/player/move_down")([&, this](const crow::request& req)
		{
			Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), req.url_params.get("username"));
			return PlayerMoveDown(*player, req);
		});
	CROW_ROUTE(m_app, "/player/move_left")([&, this](const crow::request& req)
		{
			Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), req.url_params.get("username"));
			return PlayerMoveLeft(*player, req);
		});
	CROW_ROUTE(m_app, "/player/move_right")([&, this](const crow::request& req)
		{
			Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), req.url_params.get("username"));
			return PlayerMoveRight(*player, req);
		});
	m_app.port(18080).multithreaded().run();
}
	
crow::response Routes::LoginPlayer(database::PlayerStorage& playerStorage, const std::string& username) {
	Player* player = playerStorage.GetPlayerByName(playerStorage.GetAllPlayers(), username);

	if (player != nullptr) {
		// Create a JSON response with player details
		crow::json::wvalue jsonPlayer{
			{"id", player->GetId()},
			{"username", player->GetUsername()},
			{"score", player->GetScore()},
			{"points", player->GetPoints()},
			{"firerate", player->GetFireRate()},
			{"upgrade_bs", player->GetBulletSpeedUpgrade()}
		};
		return crow::response(200, jsonPlayer);
	}

	// Player not found
	return crow::response(404, "Player not found");
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
	p.SetFireRate(1);
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
	player.SetFireRate(x);
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
			{"firerate", player.GetFireRate()},
			{"upgrade_bs", player.GetBulletSpeedUpgrade()}
			});
	}
	return crow::json::wvalue{ players_json };
}
crow::response Routes::PlayerMoveUp(Player& p, const crow::request& req)
{
	p.MoveUp();
	crow::json::wvalue mapJson;
	Map* map = p.GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::PlayerMoveDown(Player& p, const crow::request& req) {
	p.MoveDown();
	crow::json::wvalue mapJson;
	Map* map = p.GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::PlayerMoveLeft(Player& p, const crow::request& req) {
	p.MoveLeft();
	crow::json::wvalue mapJson;
	Map* map = p.GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::PlayerMoveRight(Player& p, const crow::request& req) {
	p.MoveRight();
	crow::json::wvalue mapJson;
	Map* map = p.GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::GetBulletsFromPlayer(Player& p, const crow::request& req)
{
	crow::json::wvalue bulletsJson = p.BulletsToJson();
	return bulletsJson;
}