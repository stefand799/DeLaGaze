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


	/// <summary>
	/// Method which prepares the game for running, generating map, initializing players, and starting the game loop in "run".
	/// Will probably have parameters in near future for things like connections, player count, seed, etc.
	/// </summary>
	void start();


	void update();
	void checkCollisions();
	void markForDestruction(Object* object);
	void removeDestroyedObjects();

	bool isWithinBounds(const int& i, const int& j) const;
private:
	//Methods
	
	/// <summary>
	/// Method that contains the game loop.
	/// The game loop is where all the action happens
	/// </summary>
	void run();
	
	//Constants

	

	//Atributes
	Map m_map;
	std::vector<Player*> m_players;
	std::vector<std::shared_ptr<Bullet>>m_bullets;//We could probably use shared_ptr so the bullets get deleted immediately when they go out of scope
	std::vector<Object*> m_markedForDestruction; //Object*, because currently I have no clue if we're going to alter additional logic and would be optimal not to immediately delete them when they get out of scope
	//also I'm inadequately educated so I can't call the shots here yet


	bool m_isRunning;
	
	//DEBUG METHODS:
	
};