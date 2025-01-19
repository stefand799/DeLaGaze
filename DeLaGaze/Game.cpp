#include "Game.h"


Game::Game() :
	m_isRunning{false},
	m_targetFrameDuration{ std::chrono::nanoseconds(1000000000 / kMaxFps) },
	m_startGameTime{Clock::now()}
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	m_shortestSleepTime = Nsec(1000000 / 64);
#elif defined(__unix__)
#endif 

}

Game::~Game()
{

}

void Game::Start()
{

	m_isRunning = true;
	m_lastFrameTime = std::chrono::high_resolution_clock::now();
	m_hasDeathmatchStarted = false;

	m_startGameTime = Clock::now();

	this->Run();

}

void Game::SetGameMode(GameMode gameMode) {
	m_mode = gameMode;
}

void Game::Update(){

	std::for_each(m_bullets.begin(), m_bullets.end(), [&](const std::shared_ptr<Bullet>& bullet) {bullet->Move(m_deltaTime); });
	std::for_each(m_players.begin(), m_players.end(), [&](const std::shared_ptr<Player>& player) {player->Move(m_deltaTime); });
	
	
	HandleCollisions();

	if (CheckEndCondition()) {
		m_isRunning = false;
		return;
	}

	/*if (!m_hasDeathmatchStarted)
		CheckDeathmatchCondition();
	else
		m_map.Shrink();*/
}

void Game::HandleCollisions()
{
	for (std::vector<std::shared_ptr<Bullet>>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++) {
		if (*it == nullptr) continue;
		HandleBulletToPlayerCollisions(*it);
		HandleBulletToBorderCollisions(*it);
		HandleBulletToWallCollisions(*it);
		HandleBulletToBulletCollisions(it);
	}
	RemoveDestroyedObjects();
}


void Game::HandleBulletToWallCollisions(std::shared_ptr<Bullet>& bullet)
{
	float x = bullet->GetX();
	float y = bullet->GetY();
	float xSpeed = bullet->GetXSpeed();
	float ySpeed = bullet->GetYSpeed();
	float radius = bullet->GetRadius();

	if (x<0 || x > m_map.GetMapWidth() || y < 0 || y > m_map.GetMapHeight()) {
		return;
	}
	else
	{
		std::shared_ptr<Object> collidedToObject = m_map[static_cast<int>(y)][static_cast<int>(x)];
		Object::ObjectType type = collidedToObject->GetType();
		if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock || type == Object::ObjectType::DeadlyBlock) 
			m_collisions.emplace(bullet, collidedToObject, 0 );
	}

	if (xSpeed < 0.0f) {
		int nextBlockX = static_cast<int>(x);
		float t = -1;
		do {
			if (t > 0.0f) {
				std::shared_ptr<Object> collidedToObject = m_map[static_cast<int>(y)][nextBlockX];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock || type == Object::ObjectType::DeadlyBlock)
					m_collisions.emplace(bullet, collidedToObject, t);
			}
			t = (nextBlockX-- -x + radius) / xSpeed;
		} while (t < m_deltaTime && nextBlockX >= 0);
	}
	if (xSpeed > 0.0f) {
		int nextBlockX = static_cast<int>(x);
		float t = -1;
		do {
			if (t > 0.0f) {
				std::shared_ptr<Object> collidedToObject = m_map[static_cast<int>(y)][nextBlockX];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock || type == Object::ObjectType::DeadlyBlock)
					m_collisions.emplace(bullet, collidedToObject, t );
			}
			t = (++nextBlockX - x - radius) / xSpeed;
		} while (t < m_deltaTime && nextBlockX < m_map.GetMapWidth());
	}

	if (ySpeed < 0.0f) {
		int nextBlockY = static_cast<int>(y);
		float t = -1;
		do {
			if (t > 0.0f) {
				std::shared_ptr<Object> collidedToObject = m_map[nextBlockY][static_cast<int>(x)];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock || type == Object::ObjectType::DeadlyBlock)
					m_collisions.emplace(bullet, collidedToObject, t );
			}
			t = (nextBlockY-- - y + radius) / ySpeed;
		} while (t < m_deltaTime && nextBlockY >= 0);
	}
	if (ySpeed > 0.0f) {
		int nextBlockY = static_cast<int>(y);
		float t = -1;
		do {
			if (t > 0.0f) {
				std::shared_ptr<Object> collidedToObject = m_map[nextBlockY][static_cast<int>(x)];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock || type == Object::ObjectType::DeadlyBlock)
					m_collisions.emplace(bullet, collidedToObject, t);
			}
			t = (++nextBlockY - y - radius) / ySpeed;
		} while (t < m_deltaTime && nextBlockY < m_map.GetMapHeight());
	}
}




void Game::HandleBulletToBorderCollisions(std::shared_ptr<Bullet>& bullet)
{
	float x = bullet->GetX();
	float y = bullet->GetY();
	float xSpeed = bullet->GetXSpeed();
	float ySpeed = bullet->GetYSpeed();
	float radius = bullet->GetRadius();

	if (x < 0.0f || x > m_map.GetMapWidth()) {
		m_collisions.emplace(bullet, 0);
		return;
	}
	if (y < 0.0f || y > m_map.GetMapHeight()) {
		m_collisions.emplace(bullet, 0);
		return;
	}

	float tx = -1 , ty = -1;
	if (xSpeed < 0.0f) 
		tx = (0 - x + radius) / xSpeed;

	if (xSpeed > 0.0f) 
		tx = (m_map.GetMapWidth() - x - radius) / xSpeed;
	
	if (ySpeed < 0.0f) 
		ty = (0 - y + radius) / ySpeed;
	
	if (ySpeed > 0.0f) 
		ty = (m_map.GetMapHeight() - y - radius) / ySpeed;
	

	if (tx > 0 && tx <= m_deltaTime) {
		m_collisions.emplace(bullet,tx);
		return;
	}
	if (ty > 0 && ty <= m_deltaTime) {
		m_collisions.emplace(bullet,ty);
		return;
	}
}

void Game::HandleBulletToBulletCollisions(std::vector<std::shared_ptr<Bullet>>::iterator& bulletIterator)
{
	std::shared_ptr<Bullet> bullet = *bulletIterator;
	for (std::vector<std::shared_ptr<Bullet>>::iterator it = bulletIterator + 1; it != m_bullets.end(); it++) {
		std::shared_ptr<Bullet> otherBullet = *it;
		ObjectCollision collision{ GetBulletToBulletColision(bullet, otherBullet) };
		if (collision.time >= 0 && collision.time <= m_deltaTime) 
			m_collisions.emplace(std::move(collision));
	}
}

void Game::HandleBulletToPlayerCollisions(std::shared_ptr<Bullet>& bullet)
{
	for (std::shared_ptr<Player>& player : m_players) {
		if (!player) continue;
		if (player->GetHp() <= 0) continue;
		ObjectCollision collision{ player->GetBulletToPlayerColision(bullet) };
		if (collision.time >= 0 && collision.time <= m_deltaTime)
			m_collisions.push(std::move(collision));
	}
}

void Game::RemoveDestroyedObjects()
{
	std::unordered_set<std::shared_ptr<Object>> destroyedObjects;
	while (!m_collisions.empty()) {
		ObjectCollision currCollision = m_collisions.top();
		m_collisions.pop();
		if (currCollision.time < 0.0f || currCollision.time == NAN) continue;
		if (currCollision.isBorderCollision) {
			if (currCollision.first) {
				if (destroyedObjects.find(currCollision.first) == destroyedObjects.end()) {
					destroyedObjects.insert(currCollision.first);
				}
			}
			continue;
		}
		
		if (currCollision.first && currCollision.second) 
			if (destroyedObjects.find(currCollision.first) == destroyedObjects.end() && destroyedObjects.find(currCollision.second) == destroyedObjects.end()) {
				destroyedObjects.insert(currCollision.first);
				destroyedObjects.insert(currCollision.second);
				if (std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(currCollision.second)) 
					if (std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(currCollision.first)) {
						std::shared_ptr<Player> bulletOwner = bullet->GetOwner();
						if(m_mode==GameMode::FFA) if(player->GetTeamId() != bulletOwner->GetTeamId()) bulletOwner->SetPoints(bulletOwner->GetPoints() + 100);
					}
			}
	}

	for (std::shared_ptr<Object> obj : destroyedObjects) {
		if (!obj) continue;
		Object::ObjectType type = obj->GetType();
		if (type == Object::ObjectType::Bullet) {
			auto it = std::find(m_bullets.begin(), m_bullets.end(), obj);
			if (it != m_bullets.end()) m_bullets.erase(it);
			continue;
		}
		if (type == Object::ObjectType::UnbreakableBlock || type == Object::ObjectType::DeadlyBlock) {
			continue;
		}
		if (std::shared_ptr<BreakableBlock> breakable = std::dynamic_pointer_cast<BreakableBlock>(obj)) {
			auto [x, y] = breakable->GetPos();
			breakable->OnBreak();
			m_map[y][x] = std::make_shared<Pathway>(std::pair<size_t,size_t>{x,y});
			continue;
		}
		if (std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(obj)) {
			player->Kill();
			continue;
		}
	}
}

crow::json::wvalue Game::MapToJson() {
	return m_map.toJson();
}

crow::json::wvalue Game::BulletsToJson()
{
	std::vector<crow::json::wvalue> jsonBullets;
	for (const auto& bullet : m_bullets)
	{
		jsonBullets.push_back(bullet->toJson());
	}
	return jsonBullets;
}

crow::json::wvalue Game::PlayersToJson()
{
	std::vector<crow::json::wvalue> jsonPlayers;
	for (const auto& player : m_players) {
		if (player->GetHp() <= 0) continue;
		jsonPlayers.push_back(player->toJson());
	}
	return jsonPlayers;
}
crow::json::wvalue Game::GameStateToJson()
{
	crow::json::wvalue gameState;

	gameState["map"] = MapToJson();

	gameState["bullets"] = BulletsToJson();

	gameState["players"] = PlayersToJson();

	return gameState;
}


void Game::Run()
{
	while (m_isRunning) {

		Clock::time_point frameInitialTimePoint = Clock::now();

		m_deltaTime = fSecDur(frameInitialTimePoint - m_lastFrameTime).count();
		m_lastFrameTime = frameInitialTimePoint;

		Update();

		Clock::time_point frameFinalTimePoint = std::chrono::high_resolution_clock::now();
		Nsec frameTime = std::chrono::duration_cast<Nsec>(frameFinalTimePoint - frameInitialTimePoint);
		Nsec sleepTime{ m_targetFrameDuration - frameTime };
		if (frameTime < m_targetFrameDuration) {
			Nsec remainingSleepTime = sleepTime;
			if(sleepTime >= Nsec((1000000/64)*2)) std::this_thread::sleep_for(sleepTime / 2);
			while (Clock::now() < frameFinalTimePoint + sleepTime);
		}


	}

	HandleEndOfGameActions();

}

void Game::CheckDeathmatchCondition()
{
	if (std::chrono::duration(Clock::now() - m_startGameTime) > kTimeUntilDeathmatch) {
		m_hasDeathmatchStarted = true;
	}

	uint16_t playersAlive = 0;
	for (std::shared_ptr<Player>& player : m_players) {
		if (player->GetHp() > 0) {
			playersAlive++;
		}
	}
	if (playersAlive <= 2) {
		m_hasDeathmatchStarted = true;
	}

	if (m_hasDeathmatchStarted) {
		m_map.InitiateShrinking();
		for (std::shared_ptr<Player>& player : m_players) {
			if (player->GetHp() > 0) {
				player->SetHp(1);
			}
		}
	}
}

bool Game::CheckEndCondition()
{
	std::unordered_map<uint8_t, int> teamHps;
	for (std::shared_ptr<Player>& player : m_players) {
		teamHps[player->GetTeamId()] += player->GetHp();
	}
	if (teamHps.size() == 0) return true;
	size_t currPos = m_teamLeaderboard.size();
	for (auto [id, hp] : teamHps) {
		if (hp == 0) {
			if (m_eliminatedTeams.find(id) != m_eliminatedTeams.end())
				continue;
			m_eliminatedTeams.insert(id);
			if (m_teamLeaderboard.size() == currPos)
				m_teamLeaderboard.push_back({ id });
			else 
				m_teamLeaderboard[currPos].push_back(id);
		}
	}
	if (m_eliminatedTeams.size() >= teamHps.size() - 1) {
		for (auto [id, hp] : teamHps) {
			if (hp != 0) {
				m_teamLeaderboard.push_back({ id });
				break;
			}
		}
		return true;
	}
	return false;
}

void Game::HandleEndOfGameActions()
{
	size_t leaderboardSize = m_teamLeaderboard.size();
	if (m_mode == GameMode::FFA) {
		if (leaderboardSize >= 1) {
			for (std::shared_ptr<Player>& player : m_players) {
				uint8_t playerTeamId = player->GetTeamId();
				if (std::find(m_teamLeaderboard[leaderboardSize - 1].begin(), m_teamLeaderboard[leaderboardSize - 1].end(), playerTeamId) != m_teamLeaderboard[leaderboardSize - 1].end()) {
					player->SetScore(player->GetScore() + 2);
					player->SetPoints(player->GetPoints() + 200);
				}
			}
		}
		if (leaderboardSize >= 2) {
			for (std::shared_ptr<Player>& player : m_players) {
				uint8_t playerTeamId = player->GetTeamId();
				if (std::find(m_teamLeaderboard[leaderboardSize - 2].begin(), m_teamLeaderboard[leaderboardSize - 2].end(), playerTeamId) != m_teamLeaderboard[leaderboardSize - 2].end()) {
					player->SetScore(player->GetScore() + 1);
				}
			}
		}
	}
}

Map& Game::GetMap() {
	return m_map;
}


bool Game::AddPlayers(const std::vector<std::shared_ptr<Player>>& players, GameMode gm) {
	if (!m_map.Generate()) {
		std::cerr << "Error: Map generation failed" << std::endl;
		return false;
	}

	std::cout << "Map generated successfully. Dimensions: " << m_map.GetMapWidth() << "x" << m_map.GetMapHeight() << std::endl;

	if (m_map.GetMapWidth() == 0 || m_map.GetMapHeight() == 0) {
		std::cerr << "Error: Invalid map dimensions after generation" << std::endl;
		return false;
	}

	try {
		m_players.resize(players.size());
		std::cout << "Resized m_players to: " << m_players.size() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error resizing players vector: " << e.what() << std::endl;
		return false;
	}

	m_mode = gm;

	struct SpawnPoint {
		int x;
		int y;
	};

	std::vector<SpawnPoint> spawnPoints = {
		{0, 0},
		{static_cast<int>(m_map.GetMapWidth() - 1), 0},
		{0, static_cast<int>(m_map.GetMapHeight() - 1)},
		{static_cast<int>(m_map.GetMapWidth() - 1), static_cast<int>(m_map.GetMapHeight() - 1)}
	};

	if(m_mode == GameMode::FFA){
		for (size_t i = 0; i < players.size() && i < 4; ++i) {
			if (players[i] != nullptr) {
				std::cout << "Adding player " << i << " at position (" << spawnPoints[i].x << ", " << spawnPoints[i].y << ")" << std::endl;
				m_players[i] = players[i];
				m_players[i]->SetPlayerInGame(
					&m_map,
					{ spawnPoints[i].x, spawnPoints[i].y },
					Direction::South,
					State::Idle,
					i,
					3
				);
				m_map[spawnPoints[i].y][spawnPoints[i].x] = m_players[i];
				std::cout << "Successfully added player " << i << " to map" << std::endl;
			}
		}
		return true;
	}
	else
	{
		for (size_t i = 0; i < players.size() && i < 4; ++i) {
			if (players[i] != nullptr) {
				std::cout << "Adding player " << i << " at position (" << spawnPoints[i].x << ", " << spawnPoints[i].y << ")" << std::endl;
				m_players[i] = players[i];
				m_players[i]->SetPlayerInGame(
					&m_map,
					{ spawnPoints[i].x, spawnPoints[i].y },
					Direction::South,
					State::Idle,
					i%2,
					1
				);
				m_map[spawnPoints[i].y][spawnPoints[i].x] = m_players[i];
				std::cout << "Successfully added player " << i << " to map" << std::endl;
			}
		}
		return true;
	}

	std::cout << "Unknown game mode" << std::endl;
	return false;
}

std::shared_ptr<Player> Game::GetPlayerByName(const std::string& username)
{
	for (auto player : m_players) {
		if (player->GetUsername() == username)
			return player;
	}
	return nullptr;
}