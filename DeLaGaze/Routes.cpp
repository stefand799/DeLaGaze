#include "Routes.h"

void Routes::Run(std::shared_ptr<database::PlayerStorage> playerStorage, std::shared_ptr<Lobby> lobby)
{
	CROW_ROUTE(m_app, "/map")([this](const crow::request& req) {
		return GetMapAsJson(req);
		});
	CROW_ROUTE(m_app, "/login/<string>").methods("GET"_method)([&, this](const crow::request& req, const std::string& username) {
		return LoginPlayer(playerStorage, username);
		});

    CROW_ROUTE(m_app,"/players")([&]() {
		return GetPlayersFromDatabase(playerStorage);
    });
    CROW_ROUTE(m_app, "/players/add").methods("POST"_method)([&,this](const crow::request& req) {
		return AddPlayerToDatabase(playerStorage, req);
        });
	CROW_ROUTE(m_app, "/players/update_firerate/<string>").methods("POST"_method)([&,this](const crow::request& req, const std::string& username)
		{
			return UpdatePlayerFirerate(playerStorage, req,username);
		});
	CROW_ROUTE(m_app, "/players/update_bullet_speed/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username)
		{
			return UpdatePlayerBulletSpeed(playerStorage, req,username);
		});
	CROW_ROUTE(m_app, "/player/move_up")([&, this](const crow::request& req)
		{
			auto player = playerStorage->GetPlayerByName(req.url_params.get("username"));
			return PlayerMoveUp(player, req);
		});
	CROW_ROUTE(m_app, "/player/move_down")([&, this](const crow::request& req)
		{
			auto player = playerStorage->GetPlayerByName(req.url_params.get("username"));
			return PlayerMoveDown(player, req);
		});
	CROW_ROUTE(m_app, "/player/move_left")([&, this](const crow::request& req)
		{
			auto player = playerStorage->GetPlayerByName(req.url_params.get("username"));
			return PlayerMoveLeft(player, req);
		});
	CROW_ROUTE(m_app, "/player/move_right")([&, this](const crow::request& req)
		{
			auto player = playerStorage->GetPlayerByName(req.url_params.get("username"));
			return PlayerMoveRight(player, req);
		});
	CROW_ROUTE(m_app, "/player/join/<string>")([&, this](const crow::request& req, const std::string& username)
		{
			return PlayerJoinLobby(playerStorage, req, username);
		});
	m_app.port(18080).multithreaded().run();
}
	

crow::response Routes::PlayerJoinLobby(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username) 
{
	auto player = playerStorage->GetPlayerByName(username);
	if (this->m_lobby->JoinLobby(player))
		return crow::response(200, "You joined lobby");
	else
		return crow::response(400, "Could not join lobby");
}

crow::response Routes::LoginPlayer(std::shared_ptr<database::PlayerStorage> playerStorage, const std::string& username) {
	auto player = playerStorage->GetPlayerByName(username);

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
	return AddPlayerToDatabase(playerStorage, crow::request{});
}

crow::response Routes::AddPlayerToDatabase(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req) {
    crow::json::rvalue body = crow::json::load(req.body);
    if (!body) {
        return crow::response(400, "Invalid JSON body");
    }
	auto p = std::make_shared<Player>(
		0,                            
		body["username"].s(),          
		0,                             
		0,                            
		1,                             
		false                         
	);
	if (playerStorage->AddPlayer(p)) {
		crow::json::wvalue responseJson;
		responseJson["id"] = p->GetId();
		responseJson["username"] = p->GetUsername();
		responseJson["score"] = p->GetScore();
		responseJson["points"] = p->GetPoints();
		responseJson["firerate"] = p->GetFireRate();
		responseJson["upgrade_bs"] = p->GetBulletSpeedUpgrade();
		return crow::response(201, "Player added successfully");
	}
	else {
		return crow::response(500, "Error adding player");
	}
}
crow::response Routes::UpdatePlayerFirerate(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username)
{
	auto player = playerStorage->GetPlayerByName(username);

	if (player->GetPoints() >= 500) {
		player->SetFireRate(player->GetFireRate() + 1);
		player->SetPoints(player->GetPoints() - 500);
		if (playerStorage->UpdatePlayer(player)) {
			return crow::response(200, "Player updated successfully");
		}
		else {
			return crow::response(500, "Error updating player");
		}
	}
	else {
		return crow::response(400, "Not enough points");
	}
}
crow::response Routes::UpdatePlayerBulletSpeed(std::shared_ptr<database::PlayerStorage> playerStorage,  const crow::request& req,const std::string& username) {
	auto player = playerStorage->GetPlayerByName(username);

	if (!player) {
		return crow::response(400, "Player not found");
	}
	if (player->GetBulletSpeedUpgrade()){
		return crow::response(200, "Upgrade already purchased");
	}
	if (player->GetScore() >= 10) {
		player->SetBulletSpeedUpgrade(true);
		if (playerStorage->UpdatePlayer(player)) {
			return crow::response(200, "Player updated successfully");
		}
		else {
			return crow::response(500, "Error updating player");
		}
	}
	else {
		return crow::response(400, "Your score is not high enough");
	}
}
crow::response Routes::GetPlayersFromDatabase(std::shared_ptr<database::PlayerStorage> playerStorage)
{
	std::vector<crow::json::wvalue> players_json;
	for (const auto& player : playerStorage->GetAllPlayers())
	{
		players_json.push_back(crow::json::wvalue{
			{"username", player->GetUsername()},
			{"score", player->GetScore()},
			{"points", player->GetPoints()},
			{"firerate", player->GetFireRate()},
			{"upgrade_bs", player->GetBulletSpeedUpgrade()}
			});
	}
	return crow::json::wvalue{ players_json };
}
crow::response Routes::PlayerMoveUp(std::shared_ptr<Player>& p, const crow::request& req)
{
	p->MoveUp();
	crow::json::wvalue mapJson;
	Map* map = p->GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::PlayerMoveDown(std::shared_ptr<Player>& p, const crow::request& req) {
	p->MoveDown();
	crow::json::wvalue mapJson;
	Map* map = p->GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::PlayerMoveLeft(std::shared_ptr<Player>& p, const crow::request& req) {
	p->MoveLeft();
	crow::json::wvalue mapJson;
	Map* map = p->GetMap();
	mapJson = map->toJson();
	return mapJson;
}
crow::response Routes::PlayerMoveRight(std::shared_ptr<Player>& p, const crow::request& req) {
	p->MoveRight();
	crow::json::wvalue mapJson;
	Map* map = p->GetMap();
	mapJson = map->toJson();
	return mapJson;
}

crow::response Routes::GetMapAsJson(const crow::request& req) {
	std::lock_guard<std::mutex> lock(m_mutex);
	crow::json::wvalue mapJson = this->m_lobby->m_game->GetMap().toJson();
	return mapJson;
}