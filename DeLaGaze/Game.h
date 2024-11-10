#pragma once

#include "Object.h"
#include "UnbreakableBlock.h"
#include "BreakableBlock.h"
#include "Pathway.h"
#include "BombTrapBlock.h"
#include "Player.h"
#include "Map.h"
#include <vector>
#include <cstdint>
#include <chrono>

class Game {
public:
	Game();
	~Game();
	void breakBlock(int i, int j);
	void addPlayer(const Player& player);
	void removePlayer(const Player& player);

private:
	//Methods
	//Constants

	const UnbreakableBlock* m_unbreakableBlock;
	const BreakableBlock* m_breakableBlock;
	const Pathway* m_pathway;
	

	//Atributes
	Map m_map;
	std::vector<Player> Player;
	


	//DEBUG METHODS:
	
};