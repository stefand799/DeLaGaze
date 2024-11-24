#include "Game.h"

Game::Game() :
	m_isRunning{false}
{

}

Game::~Game()
{
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
	m_isRunning = true;
	this->Run();
}

void Game::Update(){
	for (auto& bullet : m_bullets)
		bullet->Move();
	CheckCollisions();
}

void Game::CheckCollisions(){
	uint8_t mapWidth = m_map.GetMapWidth();
	uint8_t mapHeight = m_map.GetMapHeight();

	for (auto& bullet : m_bullets) {
		int i = bullet->GetPosition().first;
		int j = bullet->GetPosition().second;

		if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight) {
			auto& target = m_map[i][j];
			if (target->GetType() == Object::ObjectType::Pathway)
				continue; 
			else{
				if (target->GetType() == Object::ObjectType::BreakableBlock) {
					delete target;
					delete bullet;
					m_map[i][j] = new Pathway;
					/*TODO: properly push bullet out of bullets vector*/
				}
				else if (target->GetType() == Object::ObjectType::UnbreakableBlock) {
					delete bullet;
				}
				else if (target->GetType() == Object::ObjectType::Player) {
					Player* player = dynamic_cast<Player*>(target);
					if (player->GetHp() > 1) {
						player->SetHp();
						player->Respawn();
					}
					else {
						delete player;
						m_map[i][j] = new Pathway;
					}
				}
				else if (target->GetType() == Object::ObjectType::Bullet) {
					delete target;
					delete bullet;
					m_map[i][j] = new Pathway;
				}
				else if (target->GetType() == Object::ObjectType::BombTrapBlock) {
					
					for (int sI = -1; sI <= 1; ++sI) {
						for (int sJ = -1; sJ <= 1; ++sJ) {
							int gridI = i + sI;
							int gridJ = j + sJ;
							if (gridI >= 0 && gridI < mapWidth && gridJ >= 0 && gridJ < mapHeight) {
								auto& gridCell = m_map[gridI][gridJ];
								if (gridCell->GetType() == Object::ObjectType::UnbreakableBlock||gridCell->GetType()==Object::ObjectType::Pathway)
									continue;
								if (gridCell->GetType() == Object::ObjectType::Player)
								{
									Player* player = dynamic_cast<Player*>(gridCell);
									if (player->GetHp() > 1) {
										player->SetHp();
										player->Respawn();
									}
									else {
										delete player;
										m_map[gridI][gridJ] = new Pathway;
									}
								}
								if (gridCell->GetType() == Object::ObjectType::Bullet) {
									delete gridCell;
									m_map[gridI][gridJ] = new Pathway;
								}

							}
						}
					}
					
				}
			}
		}
	}
}



void Game::Run()
{
	while (m_isRunning) {
		break; //Untill we implement the game ending condition that changes m_isRunning to false
	}
}
