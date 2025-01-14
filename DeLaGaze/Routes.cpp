#include "Routes.h"

void Routes::Run(std::shared_ptr<database::PlayerStorage> playerStorage, std::shared_ptr<LobbyManager> lobbies)
{
	CROW_ROUTE(m_app, "/map/<string>").methods("GET"_method)([&, this](const crow::request& req, const std::string& username) {
		return GetMapAsJson(req, username);
		});
	CROW_ROUTE(m_app, "/login/<string>").methods("GET"_method)([&, this](const crow::request& req, const std::string& username) {
		return LoginPlayer(playerStorage, username);
		});
    CROW_ROUTE(m_app,"/players")([&]() {
		return GetPlayersFromDatabase(playerStorage);
    });
	CROW_ROUTE(m_app, "/players/update_firerate/<string>").methods("POST"_method)([&,this](const crow::request& req, const std::string& username)
		{
			return UpdatePlayerFirerate(playerStorage, req,username);
		});
	CROW_ROUTE(m_app, "/players/update_bullet_speed/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username)
		{
			return UpdatePlayerBulletSpeed(playerStorage, req,username);
		});
	CROW_ROUTE(m_app, "/move/<string>/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username, const std::string& direction)
		{
			return PlayerMove(req, username, direction);
		});
	CROW_ROUTE(m_app, "/face/<string>/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username, const std::string& direction)
		{
			return PlayerFace(req, username, direction);
		});
	CROW_ROUTE(m_app, "/join/<string>")([&, this](const crow::request& req, const std::string& username)
		{
			return PlayerJoinLobby(playerStorage, req, username);
		});
	CROW_ROUTE(m_app, "/shoot/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username) {
			return PlayerShoot(req, username);
		});
	m_app.port(18080).multithreaded().run();
}

crow::response Routes::PlayerShoot(const crow::request& req, const std::string& username) {
	m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->Shoot(m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetBullets());
	return crow::response(200, "Player shoot");
}
	
crow::response Routes::PlayerMove(const crow::request& req, const std::string& username, const std::string& direction)
{
	if (direction == "NORTH") {
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->MoveUp();
		return crow::response(200, "Player moved up");
	}
	if (direction == "SOUTH"){
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->MoveDown();
		return crow::response(200, "Player moved down");
	}
	if (direction == "EAST"){
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->MoveRight();
		return crow::response(200, "Player moved right");
	}
	if (direction == "WEST"){
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->MoveLeft();
		return crow::response(200, "Player moved left");
	}
	return crow::response(404, "Player invalid direction");
}

crow::response Routes::PlayerFace(const crow::request& req, const std::string& username, const std::string& direction)
{
	if (direction == "NORTH") {
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->FaceNorth();
		return crow::response(200, "Player faced north");
	}
	if (direction == "SOUTH") {
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->FaceSouth();
		return crow::response(200, "Player faced south");
	}
	if (direction == "EAST") {
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->FaceEast();
		return crow::response(200, "Player faced east");
	}
	if (direction == "WEST") {
		m_lobbies->GetLobbyByPlayer(username)->GetGame()->GetPlayerByName(username)->FaceWest();
		return crow::response(200, "Player faced west");
	}
	return crow::response(400, "Invalid direction");
}

crow::response Routes::PlayerJoinLobby(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username) 
{
	auto player = playerStorage->GetPlayerByName(username);
	if (this->m_lobbies->JoinALobby(player))
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

crow::response Routes::GetMapAsJson(const crow::request& req, const std::string& username) {
	std::lock_guard<std::mutex> lock(m_mutex);
	crow::json::wvalue mapJson = this->m_lobbies->GetLobbyByPlayer(username)->m_game->GetMap().toJson();
	return mapJson;
}