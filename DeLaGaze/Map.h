#pragma once
#include "Pathway.h"
#include "UnbreakableBlock.h"
#include "BreakableBlock.h"
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <random>

class Map
{
	//Constuctors and destructors
public:
	Map() = default;
	Map(const Map&) = delete;
	Map(Map&&) = delete;

	~Map() = default;

	//Getter
	const std::vector<Object*>& operator[](size_t line) const;
	//Getter and/or setter
	std::vector<Object*>& operator[](size_t line);

	//Methods
public:
	bool generate(const std::vector<Object*>& objects = std::vector<Object*>{},
		const std::vector<uint8_t>& probabilities = std::vector<uint8_t>{
			35, /*Pathway*/
			25, /*UnbreakableBlock*/
			40 /*BreakableBlock*/ },
			uint32_t seed = std::random_device{}()
			); //TODO IDEA: implementation of custom seed when creating a new game


	//Methods
private:
	void generateDimensions();
	bool generateStructures();


	//Constants
private:
	const uint8_t kMinMapWidth = 12, kMinMapHeight = 8,
		kMaxMapWidth = 20, kMaxMapHeight = 12;
	std::vector<uint8_t> kProbabilities{
		35, /*Pathway*/
		25, /*UnbreakableBlock*/
		40, /*BreakableBlock*/
	};
	const uint8_t kTotalBombCount = 3;

	//Atributes
private:
	std::vector<std::vector<Object*>> m_matrix;
	std::vector<Object*> m_objects;
	std::vector<uint8_t> m_probabilities;
	uint8_t m_mapWidth, m_mapHeight;
	uint32_t m_seed;
	std::mt19937 m_generator; // Mersenne Twister 19937 generator


	//DEBUG METHODS:
private:
	void __DEBUG_MAP_DIM__();
	void __DEBUG_MAP_SEED__();
	void __DEBUG_MAP_PRINT__();
};

