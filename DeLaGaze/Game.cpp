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
	if(!m_map.Generate()) return;
	//Creating players EXAMPLE

	//m_players.emplace_back(std::move(std::make_shared<Player>(&m_map, std::pair<int, int>{ 0, 0 }, 1, "Player1", 0, 3, true, Direction::South, State::Idle, 0 ,0 ,3)));
	//m_map[0][0] = m_players[0];

	//testing with another player!
	//m_players.emplace_back(std::move(std::make_shared<Player>(&m_map, std::pair<int, int>{ m_map.GetMapWidth() - 1, 0 }, 1, "Player2", 0, 3, true, Direction::South, State::Idle, 0 ,1, 3)));
	//m_map[0][m_map.GetMapWidth() - 1] = m_players[1];
	//m_players.emplace_back(std::move(std::make_shared<Player>(&m_map, std::pair<int, int>{ m_map.GetMapWidth() - 1, m_map.GetMapHeight() - 1 }, 1, "Player3", 0, 3, true, Direction::South, State::Idle, 0, 2, 3)));
	//m_map[m_map.GetMapHeight()-1][m_map.GetMapWidth() - 1] = m_players[2];

	m_isRunning = true;
	m_lastFrameTime = std::chrono::high_resolution_clock::now();

	//EXAMPLE PLAYER INPUT ON OTHER THREAD
	// shouldn't be needed since routes are multithreaded, but I left it here for example and refference
	//std::thread playerInputThread([this]() {
	//	GetPlayerInputs();
	//	});

	m_startGameTime = Clock::now();

	this->Run();

	//Part of the example for thread use for input from above
	//if (playerInputThread.joinable())
	//	playerInputThread.join();
}

void Game::Update(){
	//Handle player input
	//for now only one player EXAMPLE
	


	//while (!m_playerInputs.empty()) {
	//	auto [currPlayer,currInput] = m_playerInputs.front();
	//	m_playerInputs.pop();
	//	if (currInput == 27) m_isRunning = false; //escape
	//	if (currInput == 'w' || currInput == 'W') currPlayer->MoveUp();
	//	if (currInput == 'a' || currInput == 'A') currPlayer->MoveLeft();
	//	if (currInput == 's' || currInput == 'S') currPlayer->MoveDown();
	//	if (currInput == 'd' || currInput == 'D') currPlayer->MoveRight();
	//	if (currInput == 1) currPlayer->FaceNorth();
	//	if (currInput == 2) currPlayer->FaceSouth();
	//	if (currInput == 3) currPlayer->FaceWest();
	//	if (currInput == 4) currPlayer->FaceEast();
	//	if (currInput == ' ') currPlayer->Shoot(m_bullets);
	//}



	for (std::shared_ptr<Bullet>& bullet : m_bullets)
		bullet->Move(m_deltaTime);

	for (std::shared_ptr<Player>& player : m_players) {
		player->Move(m_deltaTime);
	}

	HandleCollisions();

	if (CheckEndCondition()) {
		m_isRunning = false;
		return;
	}

	if (!m_hasDeathmatchStarted)
		CheckDeathmatchCondition();
	else
		m_map.Shrink();
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
		if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
			m_collisions.emplace(std::move(ObjectCollision{ bullet, collidedToObject, 0 }));
	}

	if (xSpeed < 0.0f) {
		int nextBlockX = static_cast<int>(x);
		float t = -1;
		do {
			if (t > 0.0f) {
				std::shared_ptr<Object> collidedToObject = m_map[static_cast<int>(y)][nextBlockX];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.emplace(std::move(ObjectCollision{ bullet, collidedToObject, t }));
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
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.emplace(std::move(ObjectCollision{ bullet, collidedToObject, t }));
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
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.emplace(std::move(ObjectCollision{ bullet, collidedToObject, t }));
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
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.emplace(std::move(ObjectCollision{ bullet, collidedToObject, t }));
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
		m_collisions.emplace(std::move(ObjectCollision{ bullet, 0 }));
		return;
	}
	if (y < 0.0f || y > m_map.GetMapHeight()) {
		m_collisions.emplace(std::move(ObjectCollision{ bullet, 0 }));
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
		m_collisions.emplace(std::move(ObjectCollision{ bullet,tx }));
		return;
	}
	if (ty > 0 && ty <= m_deltaTime) {
		m_collisions.emplace(std::move(ObjectCollision{ bullet,ty }));
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
						if(player->GetTeamId() != bulletOwner->GetTeamId()) bulletOwner->SetScore(bulletOwner->GetScore() + 100);
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
		if (type == Object::ObjectType::UnbreakableBlock) {
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



crow::json::wvalue Game::BulletsToJson()
{
	std::vector<crow::json::wvalue> jsonBullets;
	for (const auto& bullet : m_bullets)
	{
		jsonBullets.push_back(bullet->toJson());
	}
	return jsonBullets;
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
	size_t eliminatedTeamsCount = 0;
	size_t currPos = m_teamLeaderboard.size();
	for (auto [id, hp] : teamHps) {
		if (hp == 0) {
			eliminatedTeamsCount++;
			if (m_teamLeaderboard.size() == currPos)
				m_teamLeaderboard.push_back({ id });
			else 
				m_teamLeaderboard[currPos].push_back(id);
		}
	}
	if (eliminatedTeamsCount >= teamHps.size() - 1) {
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
	if (leaderboardSize >= 1) {
		for (std::shared_ptr<Player>& player : m_players) {
			uint8_t playerTeamId = player->GetTeamId();
			if (std::find(m_teamLeaderboard[leaderboardSize - 1].begin(), m_teamLeaderboard[leaderboardSize - 1].end(), playerTeamId) != m_teamLeaderboard[leaderboardSize - 1].end()) {
				player->SetPoints(player->GetPoints() + 2);
			}
		}
	}
	if (leaderboardSize >= 2) {
		for (std::shared_ptr<Player>& player : m_players) {
			uint8_t playerTeamId = player->GetTeamId();
			if (std::find(m_teamLeaderboard[leaderboardSize - 2].begin(), m_teamLeaderboard[leaderboardSize - 2].end(), playerTeamId) != m_teamLeaderboard[leaderboardSize - 2].end()) {
				player->SetPoints(player->GetPoints() + 1);
			}
		}
	}
	// TODO: SAVE SCORE AND POINTS TO DATABASE
}

Map& Game::GetMap() {
	return m_map;
}

//for now only for testing with one player but in the future may collect input data from players as part of the server part
// EXAMPLE
//void Game::GetPlayerInputs()
//{
//	while(m_isRunning) {
//		char keyboardInput = _getch();
//
//		if (keyboardInput == -32) { //Arrow input
//			char arrowKey = _getch();
//			if(arrowKey == 'H') //up
//				m_playerInputs.push({ m_players[0], 1});
//			if(arrowKey == 'P') //down
//				m_playerInputs.push({ m_players[0], 2 });
//			if (arrowKey == 'K') //left
//				m_playerInputs.push({ m_players[0], 3 });
//			if (arrowKey == 'M') //right
//				m_playerInputs.push({ m_players[0], 4 });
//		}
//		else 
//			m_playerInputs.push({ m_players[0],keyboardInput});
//		if (keyboardInput == 27) break; // ESCAPE, to ensure that the loop is not run again before the other thread changes m_isRunning to false
//	}
//}