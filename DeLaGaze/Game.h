#pragma once

#include "Object.h"
#include "Block.h"
#include <vector>
#include <cstdint>
#include <random>
#include <chrono>

class Game {
public:
	Game();
	~Game();

private:
	//Methods

	//Constants
	const uint8_t kMinMapLength = 10, kMminMapWidth = 10,
		kMaxMapLength = 38, kMaxMapWidth = 21; // Magic numbers :)) taking as a reference 50px, 1920/50 = 38.4 and 1080/50 = 21.6

	//Atributes
	std::vector<std::vector<Object*>> m_map;
	uint8_t m_mapLength, m_mapWidth;
	uint32_t m_seed; //IDEA: *using a constructor* implementation of custom seed when creating a new game


	//DEBUG METHODS:
	void __DEBUG_MAP__();
};