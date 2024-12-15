#include "Game.h"


Game::Game() :
	m_isRunning{false},
	m_targetFrameDuration{ std::chrono::nanoseconds(1000000000 / m_maxFps) }
{
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	m_shortestSleepTime = Nsec(1000000 / 64);
#elif defined(__unix__)
#endif // 

}

Game::~Game()
{
	for (Bullet* bullet : m_bullets) {
		if (bullet) delete bullet;
	}
	for (Player* player : m_players) {
		if (player) delete player;
	}
}

//void Game::breakBlock(int i, int j){
//	/*TODO: change the m_map and the getters in MAP so we can actually use make_shared*/
//	auto objectType = m_map[i][j]->getType();
//	if (objectType==Object::ObjectType::Player) {
//		//TODO: respawn and reset position
//	} else if (objectType==Object::ObjectType::Bullet) {
//		//TODO: destroy both bullets
//	} else if (objectType == Object::ObjectType::UnbreakableBlock) {
//		//TODO: destroy the bullet
//	}else if (objectType == Object::ObjectType::BreakableBlock) {
//		//TODO: break the block
//		delete m_map[i][j];
//		m_map[i][j] = new Pathway;
//	}else if (objectType == Object::ObjectType::BombTrapBlock) {
//		//TODO: break all surrounding blocks; make sure we dont delete the instance itself of player
//	}
//
//}

void Game::Start()
{
	if(!m_map.Generate()) return;
	//Creating players

	delete m_map[0][0];
	m_players.emplace_back(new Player(&m_map, { 0,0 }, 1, "Player1", 0, 3, true, Direction::South, State::Idle));
	m_map[0][0] = m_players[0];

	//ONLY FOR TESTING
#ifdef DEBUG
	__DEBUG_PRINT_MAP__();
#endif // DEBUG


	m_isRunning = true;
	m_lastFrameTime = std::chrono::high_resolution_clock::now();
	std::thread playerInputThread([this]() {
		GetPlayerInputs();
		});
	this->Run();

	if (playerInputThread.joinable())
		playerInputThread.join();
	//TODO: Method that does end of game actions...
}

void Game::Update(){
	//Handle player input
	//for now only one player
	while (!m_playerInputs.empty()) {
		auto [currPlayer,currInput] = m_playerInputs.front();
		m_playerInputs.pop();
		if (currInput == 27) m_isRunning = false; //escape
		if (currInput == 'w' || currInput == 'W') currPlayer->MoveUp();
		if (currInput == 'a' || currInput == 'A') currPlayer->MoveLeft();
		if (currInput == 's' || currInput == 'S') currPlayer->MoveDown();
		if (currInput == 'd' || currInput == 'D') currPlayer->MoveRight();
		if (currInput == 1) currPlayer->FaceNorth();
		if (currInput == 2) currPlayer->FaceSouth();
		if (currInput == 3) currPlayer->FaceWest();
		if (currInput == 4) currPlayer->FaceEast();
		if (currInput == ' ') currPlayer->Shoot(m_bullets);
	}

	//Handle bullets Update;
	for (Bullet* bullet : m_bullets)
		bullet->Move(m_deltaTime);

	for (Player* player : m_players) {
		player->Move(m_deltaTime);
	}
	//Handle collisions
	//CheckCollisions();
	HandleCollisions();
}

void Game::HandleCollisions()
{
	for (std::vector<Bullet*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++) {
		if (*it == nullptr) continue;
		HandleBulletToBorderCollisions(*it);
		HandleBulletToWallCollisions(*it);
		HandleBulletToBulletCollisions(it);
	}
	RemoveDestroyedObjects();
}


void Game::HandleBulletToWallCollisions(Bullet* bullet)
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
		Object* collidedToObject = m_map[static_cast<int>(y)][static_cast<int>(x)];
		Object::ObjectType type = collidedToObject->GetType();
		if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
			m_collisions.push(ObjectCollision{ bullet, collidedToObject, 0 });
	}

	if (xSpeed < 0.0f) {
		int nextBlockX = static_cast<int>(x);
		float t = -1;
		do {
			if (t > 0.0f) {
				Object* collidedToObject = m_map[static_cast<int>(y)][nextBlockX];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.push(ObjectCollision{ bullet, collidedToObject, t });
			}
			t = (nextBlockX-- -x + radius) / xSpeed;
		} while (t < m_deltaTime && nextBlockX >= 0);
	}
	if (xSpeed > 0.0f) {
		int nextBlockX = static_cast<int>(x);
		float t = -1;
		do {
			if (t > 0.0f) {
				Object* collidedToObject = m_map[static_cast<int>(y)][nextBlockX];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.push(ObjectCollision{ bullet, collidedToObject, t });
			}
			t = (++nextBlockX - x - radius) / xSpeed;
		} while (t < m_deltaTime && nextBlockX < m_map.GetMapWidth());
	}

	if (ySpeed < 0.0f) {
		int nextBlockY = static_cast<int>(y);
		float t = -1;
		do {
			if (t > 0.0f) {
				Object* collidedToObject = m_map[nextBlockY][static_cast<int>(x)];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.push(ObjectCollision{ bullet, collidedToObject, t });
			}
			t = (nextBlockY-- - y + radius) / ySpeed;
		} while (t < m_deltaTime && nextBlockY >= 0);
	}
	if (ySpeed > 0.0f) {
		int nextBlockY = static_cast<int>(y);
		float t = -1;
		do {
			if (t > 0.0f) {
				Object* collidedToObject = m_map[nextBlockY][static_cast<int>(x)];
				Object::ObjectType type = collidedToObject->GetType();
				if (type == Object::ObjectType::BombTrapBlock || type == Object::ObjectType::BreakableBlock || type == Object::ObjectType::UnbreakableBlock) 
					m_collisions.push(ObjectCollision{ bullet, collidedToObject, t });
			}
			t = (++nextBlockY - y - radius) / ySpeed;
		} while (t < m_deltaTime && nextBlockY < m_map.GetMapHeight());
	}
}




void Game::HandleBulletToBorderCollisions(Bullet* bullet)
{
	float x = bullet->GetX();
	float y = bullet->GetY();
	float xSpeed = bullet->GetXSpeed();
	float ySpeed = bullet->GetYSpeed();
	float radius = bullet->GetRadius();

	if (x < 0.0f || x > m_map.GetMapWidth()) {
		m_collisions.push(ObjectCollision{ bullet, 0 });
		return;
	}
	if (y < 0.0f || y > m_map.GetMapHeight()) {
		m_collisions.push(ObjectCollision{ bullet, 0 });
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
		m_collisions.push(ObjectCollision{ bullet,tx });
		return;
	}
	if (ty > 0 && ty <= m_deltaTime) {
		m_collisions.push(ObjectCollision{ bullet,ty });
		return;
	}
}

void Game::HandleBulletToBulletCollisions(std::vector<Bullet*>::iterator& bulletIterator)
{
	Bullet* bullet = *bulletIterator;
	for (std::vector<Bullet*>::iterator it = bulletIterator + 1; it != m_bullets.end(); it++) {
		Bullet* otherBullet = *it;
		ObjectCollision collision{ GetBulletToBulletColision(bullet, otherBullet) };
		if (collision.time > 0 && collision.time <= m_deltaTime) m_collisions.push(collision);
	}
}

void Game::RemoveDestroyedObjects()
{
	std::unordered_set<Object*> destroyedObjects;
	while (!m_collisions.empty()) {
		ObjectCollision currCollision = m_collisions.top();
		m_collisions.pop();
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
			}
	}

	for (Object* obj : destroyedObjects) {
		if (!obj) continue;
		Object::ObjectType type = obj->GetType();
		if (type == Object::ObjectType::Bullet) {
			auto it = std::find(m_bullets.begin(), m_bullets.end(), obj);
			if (it != m_bullets.end()) m_bullets.erase(it);
			delete obj;
			continue;
		}
		//Handle other types of objects...
		if (type == Object::ObjectType::UnbreakableBlock) {
			continue;
		}
		if (BreakableBlock* breakable = dynamic_cast<BreakableBlock*>(obj)) {
			auto [x, y] = breakable->GetPos();
			breakable->OnBreak();
			delete m_map[y][x];
			m_map[y][x] = new Pathway{ {x,y} };
			continue;
		}
	}
}







//void Game::CheckCollisions(){
//	uint8_t mapWidth = m_map.GetMapWidth();
//	uint8_t mapHeight = m_map.GetMapHeight();
//
//	for (auto& bullet : m_bullets) {
//		int i = bullet->GetY();
//		int j = bullet->GetX();
//
//		if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight) {
//			auto& target = m_map[i][j];
//			if (target->GetType() == Object::ObjectType::Pathway)
//				continue; 
//			else{
//				if (target->GetType() == Object::ObjectType::BreakableBlock) {
//					delete target;
//					delete bullet;
//					m_map[i][j] = new Pathway;
//					/*TODO: properly push bullet out of bullets vector*/
//				}
//				else if (target->GetType() == Object::ObjectType::UnbreakableBlock) {
//					delete bullet;
//				}
//				else if (target->GetType() == Object::ObjectType::Player) {
//					Player* player = dynamic_cast<Player*>(target);
//					if (player->GetHp() > 1) {
//						player->SetHp();
//						player->Respawn();
//					}
//					else {
//						delete player;
//						m_map[i][j] = new Pathway;
//					}
//				}
//				else if (target->GetType() == Object::ObjectType::Bullet) {
//					delete target;
//					delete bullet;
//					m_map[i][j] = new Pathway;
//				}
//				else if (target->GetType() == Object::ObjectType::BombTrapBlock) {
//					
//					for (int sI = -1; sI <= 1; ++sI) {
//						for (int sJ = -1; sJ <= 1; ++sJ) {
//							int gridI = i + sI;
//							int gridJ = j + sJ;
//							if (gridI >= 0 && gridI < mapWidth && gridJ >= 0 && gridJ < mapHeight) {
//								auto& gridCell = m_map[gridI][gridJ];
//								if (gridCell->GetType() == Object::ObjectType::UnbreakableBlock||gridCell->GetType()==Object::ObjectType::Pathway)
//									continue;
//								if (gridCell->GetType() == Object::ObjectType::Player)
//								{
//									Player* player = dynamic_cast<Player*>(gridCell);
//									if (player->GetHp() > 1) {
//										player->SetHp();
//										player->Respawn();
//									}
//									else {
//										delete player;
//										m_map[gridI][gridJ] = new Pathway;
//									}
//								}
//								if (gridCell->GetType() == Object::ObjectType::Bullet) {
//									delete gridCell;
//									m_map[gridI][gridJ] = new Pathway;
//								}
//
//							}
//						}
//					}
//					
//				}
//			}
//		}
//	}
//}



void Game::Run()
{
	//Clock::time_point lastSecond = Clock::now();
	//int x = 0;

	while (m_isRunning) {

		Clock::time_point frameInitialTimePoint = Clock::now();

		m_deltaTime = fSecDur(frameInitialTimePoint - m_lastFrameTime).count();
		m_lastFrameTime = frameInitialTimePoint;

		bool isAtleatOnePlayerMove = false;
		for (Player* player : m_players) {
			if (player->GetMovingState()) {
				isAtleatOnePlayerMove = true;
				break;
			}
		}
		if (!m_playerInputs.empty() || !m_bullets.empty() || isAtleatOnePlayerMove)
		{
			Update();
#ifdef DEBUG
			//HERE ONLY FOR MOMENTARY DISPLAY OF MAP FOR TESTING
			__DEBUG_PRINT_MAP__();
#endif // DEBUG


		}

		Clock::time_point frameFinalTimePoint = std::chrono::high_resolution_clock::now();
		Nsec frameTime = std::chrono::duration_cast<Nsec>(frameFinalTimePoint - frameInitialTimePoint);
		Nsec sleepTime{ m_targetFrameDuration - frameTime };
		if (frameTime < m_targetFrameDuration) {
			Nsec remainingSleepTime = sleepTime;
			//TODO: add hybrid frame limiter using std::this_thread::sleep_for(SLEEPTIME_OR_SOME_DURATION) before the current busy-waiting
			// but find a way to work around or with the windows 64hz resolution clock that limits the sleep precision without overexagerating context switching
			if(sleepTime >= Nsec((1000000/64)*2)) std::this_thread::sleep_for(sleepTime / 2); //the 64 comes from the 64hz windows clock
			while (Clock::now() < frameFinalTimePoint + sleepTime); //busy waiting
		}


	}
}

//for now only for testing with one player but in the future may collect input data from players as part of the server part
void Game::GetPlayerInputs()
{
	while(m_isRunning) {
		char keyboardInput = _getch();

		if (keyboardInput == -32) { //Arrow input
			char arrowKey = _getch();
			if(arrowKey == 'H') //up
				m_playerInputs.push({ m_players[0], 1});
			if(arrowKey == 'P') //down
				m_playerInputs.push({ m_players[0], 2 });
			if (arrowKey == 'K') //left
				m_playerInputs.push({ m_players[0], 3 });
			if (arrowKey == 'M') //right
				m_playerInputs.push({ m_players[0], 4 });
		}
		else 
			m_playerInputs.push({ m_players[0],keyboardInput});
		if (keyboardInput == 27) break; // ESCAPE, to ensure that the loop is not run again before the other thread changes m_isRunning to false
	}
}

void Game::__DEBUG_PRINT_MAP__()
{
	system("cls");
	for (size_t line = 0; line < m_map.GetMapHeight(); ++line) {
		for (size_t column = 0; column < m_map.GetMapWidth(); ++column) {
			bool isBulletHere = false;
			for (const Bullet* currBullet : m_bullets) {
				if ((int)currBullet->GetY() == line && (int)currBullet->GetX() == column) {
					currBullet->Print();
					isBulletHere = true;
					break;
				}
			}
			if(!isBulletHere) m_map[line][column]->Print();
		}
		std::cout << "\n";
	}
}
