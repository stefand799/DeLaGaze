#pragma once
#include "Object.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

class Map
{
public:
	Map(uint32_t seed = std::random_device{}()); //TODO IDEA: implementation of custom seed when creating a new game
	~Map() = default;

	//Getter
	const std::vector<Object*>& operator[](size_t line) const;
	//Getter and/or setter
	std::vector<Object*>& operator[](size_t line);

	bool generate(const std::vector<Object*>& objects);


	//Methods
private:
	void generateDimensions();


	//Constants
private:
	const uint8_t kMinMapWidth = 12, kMinMapHeight = 8,
		kMaxMapWidth = 20, kMaxMapHeight = 12;

	//Atributes
private:
	std::vector<std::vector<Object*>> m_matrix;
	std::vector<Object*> m_objects;
	uint8_t m_mapWidth, m_mapHeight;
	uint32_t m_seed;
	std::mt19937 m_generator; // Mersenne Twister 19937 generator


	//DEBUG METHODS:
private:
	void __DEBUG_MAP_DIM__();
	void __DEBUG_MAP_SEED__();
};

