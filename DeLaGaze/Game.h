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


	void update();
	void checkCollisions();
	void markForDestruction(Object* object);
	void removeDestroyedObjects();
private:
	//Methods
	//Constants

	const UnbreakableBlock* m_unbreakableBlock;
	const BreakableBlock* m_breakableBlock;
	const Pathway* m_pathway;
	

	//Atributes
	Map m_map;
	std::vector<Player> Player;
	std::vector<std::shared_ptr<Bullet>>m_bullets;//We could probably use shared_ptr so the bullets get deleted immediately when they go out of scope
	std::vector<Object*> m_markedForDestruction; //Object*, because currently I have no clue if we're going to alter additional logic and would be optimal not to immediately delete them when they get out of scope
	//also I'm inadequately educated so I can't call the shots here yet
	
	//DEBUG METHODS:
	
};