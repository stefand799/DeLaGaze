#pragma once
#include "Pathway.h"
#include "UnbreakableBlock.h"
#include "BreakableBlock.h"
#include "BombTrapBlock.h"
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <chrono>
#include <random>

class Map
{
	//Constuctors and destructors
public:
	Map();
	Map(const Map&) = delete;
	Map(Map&&) = delete;

	~Map();

	//Getter
	const std::vector<Object*>& operator[](size_t line) const;
	//Getter and/or setter
	std::vector<Object*>& operator[](size_t line);

	//Methods
public:
	bool generate(
		const std::vector<uint8_t>& probabilities = std::vector<uint8_t>{
			35, /*Pathway*/
			25, /*UnbreakableBlock*/
			40 /*BreakableBlock*/ },
			uint32_t seed = std::random_device{}()
			); //TODO IDEA: implementation of custom seed when creating a new game


	//Methods
private:
	bool verifyProbabilities();
	void generateDimensions();
	bool generateStructures();
	void makeCornerPathway(size_t x, size_t y, std::vector<Object**>& breakableBlocksVector);
	void placeBombs(std::vector<Object**>& breakableBlocksVector);

	// Using Dijkstra to find the path with smallest number of UnbreakableBlocks to change if players are isolated
	// This uses the nested BestPathNode class
	std::vector<std::vector<std::pair<size_t, size_t>>> findBestPath(std::pair<size_t, size_t> start, std::pair<size_t,size_t> end);
	// Changing the UnbreakableBlocks into BreakableBlocks along the found path with findBestPath
	void breakUnbreakableOnBestPath(std::vector<std::vector<std::pair<size_t, size_t>>> path, std::pair<size_t, size_t> start, std::pair<size_t, size_t> end);

	// Nested Node class for Dijkstra
private:
	class BestPathNode {
	public:
		BestPathNode(std::pair<size_t, size_t> pos, uint32_t normalBlocksCount, uint32_t unbreakableBlocksCount) :
			m_pos{ pos },
			m_normalBlocksCount {normalBlocksCount},
			m_unbreakableBlocksCount{unbreakableBlocksCount} {}
		bool operator>(const BestPathNode& other) const {
			if (this->m_unbreakableBlocksCount != other.m_unbreakableBlocksCount) {
				// The count of unbreakableBlocks should be minimized
				return this->m_unbreakableBlocksCount > other.m_unbreakableBlocksCount;
			}
			return this->m_normalBlocksCount > other.m_normalBlocksCount;
		}
		std::pair<size_t, size_t> getPosition() const { return m_pos; }
		uint32_t getNormalBlcoksCount() const { return m_normalBlocksCount; }
		uint32_t getUnbreakableBlcoksCount() const { return m_unbreakableBlocksCount; }

	private:
		std::pair<size_t, size_t> m_pos;
		uint32_t m_normalBlocksCount;
		uint32_t m_unbreakableBlocksCount;
	};



	//Constants
private:
	const uint8_t kMinMapWidth { 12 }, kMinMapHeight { 8 },
		kMaxMapWidth { 20 }, kMaxMapHeight  { 12 };
	std::vector<uint8_t> kProbabilities{
		35, /*Pathway*/
		25, /*UnbreakableBlock*/
		40, /*BreakableBlock*/
	};
	const uint8_t kTotalBombCount{ 3 };

	//Atributes
private:
	std::vector<std::vector<Object*>> m_matrix;
	std::vector<uint8_t> m_probabilities;
	uint8_t m_mapWidth, m_mapHeight;
	uint32_t m_seed;
	std::mt19937 m_generator; // Mersenne Twister 19937 generator

	bool m_alreadyGenerated : 1;

	//DEBUG METHODS:
private:
	void __DEBUG_MAP_DIM__();
	void __DEBUG_MAP_SEED__();
	void __DEBUG_MAP_PRINT__();
};

