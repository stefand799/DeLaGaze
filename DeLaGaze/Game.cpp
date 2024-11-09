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
