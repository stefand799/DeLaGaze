#include "Routes.h"

void Routes::Run(std::shared_ptr<database::PlayerStorage> playerStorage, std::shared_ptr<LobbyManager> lobbies)
{
	CROW_ROUTE(m_app, "/getgamestate/<string>").methods("GET"_method)([&, this](const crow::request& req, const std::string& username) {
		return GetGameStateAsJson(req, username);
		});
	CROW_ROUTE(m_app, "/gamestarted/<string>").methods("GET"_method)([&, this](const crow::request& req, const std::string& username) {
		return GameStarted(req, username);
		});
	CROW_ROUTE(m_app, "/login/<string>").methods("GET"_method)([&, this](const crow::request& req, const std::string& username) {
		return LoginPlayer(playerStorage, username);
		});
	CROW_ROUTE(m_app, "/players")([&]() {
		return GetPlayersFromDatabase(playerStorage);
		});
	CROW_ROUTE(m_app, "/players/update_firerate/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username)
		{
			return UpdatePlayerFirerate(playerStorage, req, username);
		});
	CROW_ROUTE(m_app, "/players/update_bullet_speed/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username)
		{
			return UpdatePlayerBulletSpeed(playerStorage, req, username);
		});
	CROW_ROUTE(m_app, "/move/<string>/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username, const std::string& direction)
		{
			return PlayerMove(req, username, direction);
		});
	CROW_ROUTE(m_app, "/face/<string>/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username, const std::string& direction)
		{
			return PlayerFace(req, username, direction);
		});
	CROW_ROUTE(m_app, "/join/<string>/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& gameMode, const std::string& username)
		{
			return PlayerJoinLobby(playerStorage, req, gameMode, username);
		});
	CROW_ROUTE(m_app, "/shoot/<string>").methods("POST"_method)([&, this](const crow::request& req, const std::string& username) {
		return PlayerShoot(req, username);
		});
	m_app.port(18080).multithreaded().run();
}

crow::response Routes::GameStarted(const crow::request& req, const std::string& username) {
	auto lobby = this->m_lobbies->GetLobbyByPlayer(username);
	if (!lobby)
		return crow::response(400, "Lobby not found for player!");

	auto game = lobby->GetGame();
	if (!game)
		return crow::response(400, "Game hasn't been initialized!");

	if (game->IsRunning())
		return crow::response(200, "Game has started!");

	return crow::response(303, "Game loading!");
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

crow::response Routes::PlayerJoinLobby(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& gameMode, const std::string& username) {
	auto persistentPlayer = playerStorage->GetPlayerByName(username);
	if (!persistentPlayer) {
		return crow::response(400, "Player not found");
	}

	GameMode mode;
	if (gameMode == "ffa") {
		mode = GameMode::FFA;
	}
	else if (gameMode == "teams") {
		mode = GameMode::TEAMS;
	}
	else {
		return crow::response(400, "Invalid game mode");
	}
	auto lobby = m_lobbies->JoinALobby(persistentPlayer, mode);
	if (lobby) {
		return crow::response(200, "You joined lobby with ID: " + lobby->GetId());
	}
	else {
		return crow::response(500, "Could not join lobby");
	}
}

crow::response Routes::LoginPlayer(std::shared_ptr<database::PlayerStorage> playerStorage, const std::string& username) {
	if (username.empty()) {
		return crow::response(400, "Username is required");
	}

	auto player = playerStorage->GetPlayerByName(username);
	if (player != nullptr) {
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

	auto newPlayer = std::make_shared<Player>(
		0,                            
		username,                    
		0,                            
		0,                           
		1,                            
		false                        
	);

	if (playerStorage->AddPlayer(newPlayer)) {
		crow::json::wvalue responseJson{
			{"id", newPlayer->GetId()},
			{"username", newPlayer->GetUsername()},
			{"score", newPlayer->GetScore()},
			{"points", newPlayer->GetPoints()},
			{"firerate", newPlayer->GetFireRate()},
			{"upgrade_bs", newPlayer->GetBulletSpeedUpgrade()}
		};
		return crow::response(201, responseJson);
	}
	else {
		return crow::response(500, "Error adding player");
	}
}

crow::response Routes::UpdatePlayerFirerate(std::shared_ptr<database::PlayerStorage> playerStorage, const crow::request& req, const std::string& username)
{
	auto player = playerStorage->GetPlayerByName(username);

	if (player->GetFireRate() == 4)
		return crow::response(201, "Upgrade already maxed out");

	if (player->GetPoints() >= 500) {
		player->SetFireRate(player->GetFireRate() + 1);
		player->SetPoints(player->GetPoints() - 500);
		if (playerStorage->UpdatePlayer(player)) {
			crow::json::wvalue response_json;
			response_json["points"] = player->GetPoints();
			response_json["fireRate"] = player->GetFireRate();
			response_json["message"] = "Player updated successfully!";
			return crow::response(200, response_json);
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
		return crow::response(201, "Upgrade already maxed out");
	}
	if (player->GetScore() >= 10) {
		player->SetBulletSpeedUpgrade(true);
		if (playerStorage->UpdatePlayer(player)) {
			crow::json::wvalue response_json;
			response_json["score"] = player->GetScore();
			response_json["upgrade_bs"] = player->GetBulletSpeedUpgrade();
			response_json["message"] = "Player updated successfully!";
			return crow::response(200, response_json);
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

crow::response Routes::GetGameStateAsJson(const crow::request& req, const std::string& username) {
	if (!IsGameActive(username)) {
		return crow::response(301, "Game has ended");
	}

	auto lobby = m_lobbies->GetLobbyByPlayer(username);
	auto game = lobby->GetGame();

	crow::json::wvalue gameStateJson = game->GameStateToJson();
	return crow::response(200, gameStateJson.dump());
}

bool Routes::IsGameActive(const std::string& username) {
	auto lobby = m_lobbies->GetLobbyByPlayer(username);
	if (!lobby) return false;

	auto game = lobby->GetGame();
	if (!game) return false;

	std::cout << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning() << game->IsRunning();
	return game->IsRunning();
}