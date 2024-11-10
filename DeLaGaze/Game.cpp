#include "Game.h"

Game::Game()
{
	m_breakableBlock = new BreakableBlock;
	m_unbreakableBlock = new UnbreakableBlock;
	m_pathway = new Pathway;
}

Game::~Game()
{
}

void Game::breakBlock(int i, int j){
	/*TODO: change the m_map and the getters in MAP so we can actually use make_shared*/
	delete m_map[i][j];
	m_map[i][j] = new Pathway();
	/*TODO: check for additional conditions in the case of BOMB*/
}
