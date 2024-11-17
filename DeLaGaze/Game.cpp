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

void Game::start()
{
	if(!m_map.generate()) return;
	m_isRunning = true;
	this->run();
}

void Game::update(){
	for (auto& bullet : m_bullets)
		bullet->move();
	checkCollisions();
	removeDestroyedObjects();
}

void Game::checkCollisions(){
	uint8_t mapWidth = m_map.getMapWidth();
	uint8_t mapHeight = m_map.getMapHeight();

	for (auto& bullet : m_bullets) {
		int i = bullet->getPosition().first;
		int j = bullet->getPosition().second;

		if (i >= 0 && i < mapWidth && j >= 0 && j < mapHeight) {
			auto& target = m_map[i][j];
			if (target->getType() == Object::ObjectType::Pathway)
				continue; 
			else{
				if (target->getType() == Object::ObjectType::BreakableBlock) {
					//m_map[i][j] = std::make_shared<Pathway>;
					delete m_map[i][j];
					m_map[i][j] = new Pathway;
				}
				else if (target->getType() == Object::ObjectType::Player)
				{/*TODO: !!! if(viata=0) -> delete
						else functie de RESPAWN, scade o viata si se reseteaza pozitia pe spawnpointul specific fiecaruia*/
				}
				else if (target->getType() == Object::ObjectType::BombTrapBlock) {
					
					for (int sI = -1; sI <= 1; ++sI) {
						for (int sJ = -1; sJ <= 1; ++sJ) {
							int gridI = i + sI;
							int gridJ = j + sJ;
							if (gridI >= 0 && gridI < mapWidth && gridJ >= 0 && gridJ < mapHeight) {
								auto& gridCell = m_map[gridI][gridJ];
								if (gridCell->getType() == Object::ObjectType::UnbreakableBlock||gridCell->getType()==Object::ObjectType::Pathway)
									continue;
								if (gridCell->getType() == Object::ObjectType::Player)
								{/*TODO: !!! if(viata=0) -> delete
										else functie de RESPAWN, scade o viata si se reseteaza pozitia pe spawnpointul specific fiecaruia*/
								}
								if (gridCell->getType() == Object::ObjectType::Bullet)
										markForDestruction(gridCell);

								delete m_map[gridI][gridJ];
								m_map[gridI][gridJ] = new Pathway;

							}
						}
					}
					
				}
				//bullet is destroyed as long as the block isnt pathway
				markForDestruction(bullet.get());
			}
		}
	}
}

void Game::markForDestruction(Object* object){
	m_markedForDestruction.push_back(object);
}

void Game::removeDestroyedObjects(){
	m_markedForDestruction.clear();
}

void Game::run()
{
	while (m_isRunning) {
		break; //Untill we implement the game ending condition that changes m_isRunning to false
	}
}
