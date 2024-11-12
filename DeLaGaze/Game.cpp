#include "Game.h"

Game::Game()
{

}

Game::~Game()
{
}

void Game::breakBlock(int i, int j){
	/*TODO: change the m_map and the getters in MAP so we can actually use make_shared*/
	auto objectType = m_map[i][j]->getType();
	if (objectType==Object::ObjectType::Player) {
		//TODO: respawn and reset position
	} else if (objectType==Object::ObjectType::Bullet) {
		//TODO: destroy both bullets
	} else if (objectType == Object::ObjectType::UnbreakableBlock) {
		//TODO: destroy the bullet
	}else if (objectType == Object::ObjectType::BreakableBlock) {
		//TODO: break the block
		delete m_map[i][j];
		m_map[i][j] = new Pathway;
	}else if (objectType == Object::ObjectType::BombTrapBlock) {
		//TODO: break all surrounding blocks; make sure we dont delete the instance itself of player
	}

}
