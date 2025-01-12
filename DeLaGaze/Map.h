#pragma once
#include "Pathway.h"
#include "UnbreakableBlock.h"
#include "BreakableBlock.h"
#include "BombTrapBlock.h"
#include "Object.h"
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
	Map(const Map&) = default;
	Map(Map&&) = delete;
	crow::json::wvalue toJson();
	std::string ObjectTypeToString(int i, int j);
	~Map() = default;


	//Getter
	const std::vector<std::shared_ptr<Object>>& operator[](size_t line) const;
	
	//Getter and/or setter
	std::vector<std::shared_ptr<Object>>& operator[](size_t line);

	uint8_t GetMapWidth() const { return m_mapWidth; };
	uint8_t GetMapHeight() const { return m_mapHeight; };

	//Methods
public:

	void Shrink();

	bool Generate(
		const std::vector<uint8_t>& probabilities = std::vector<uint8_t>{
			35, /*Pathway*/
			25, /*UnbreakableBlock*/
			40 /*BreakableBlock*/ },
			// The values above are for release, the values below will be used for testing the movement of player
			//70, /*Pathway*/
			//10, /*UnbreakableBlock*/
			//20 /*BreakableBlock*/ },
			uint32_t seed = std::random_device{}()
			); //TODO IDEA: implementation of custom seed when creating a new game


	//Methods
private:
	bool VerifyProbabilities();
	void GenerateDimensions();
	bool GenerateStructures();
	void MakeCornerPathway(size_t x, size_t y, std::vector<std::shared_ptr<Object>*>& breakableBlocksVector);
	void PlaceBombs(std::vector<std::shared_ptr<Object>*>& breakableBlocksVector);

	

	std::vector<std::vector<std::pair<size_t, size_t>>> FindBestPath(std::pair<size_t, size_t> start, std::pair<size_t,size_t> end);
	// Changing the UnbreakableBlocks into BreakableBlocks along the found path with findBestPath
	void BreakUnbreakableOnBestPath(std::vector<std::vector<std::pair<size_t, size_t>>> path, std::pair<size_t, size_t> start, std::pair<size_t, size_t> end);

private:
	//Usings
	using Clock = std::chrono::high_resolution_clock;

	// Nested Node class for Dijkstra
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
		std::pair<size_t, size_t> GetPosition() const { return m_pos; }
		uint32_t GetNormalBlocksCount() const { return m_normalBlocksCount; }
		uint32_t GetUnbreakableBlocksCount() const { return m_unbreakableBlocksCount; }

	private:
		std::pair<size_t, size_t> m_pos;
		uint32_t m_normalBlocksCount;
		uint32_t m_unbreakableBlocksCount;
	};



	//Constants
private:
	const uint8_t kMinMapWidth { 12 }, kMinMapHeight { 8 },
		kMaxMapWidth { 20 }, kMaxMapHeight  { 12 };
	const uint8_t kTotalBombCount{ 3 };

	const std::chrono::seconds kShrinkCooldown = std::chrono::seconds(2);

	//Atributes
private:
	/*TODO: change matrix to shared_ptr, and all pertaining methods to accomodate for the new usage (matrix.get().Method())*/
	std::vector<std::vector<std::shared_ptr<Object>>> m_matrix;
	std::vector<uint8_t> m_probabilities;
	uint8_t m_mapWidth, m_mapHeight;
	uint32_t m_seed;
	std::mt19937 m_generator; // Mersenne Twister 19937 generator

	Clock::time_point m_lastShrinkTime;
	int8_t m_shrinkOrder;

	bool m_isGenerated : 1;

public:
	bool IsWithinBounds(const int& i, const int& j) const;
	//DEBUG METHODS:
private:
	void __DEBUG_MAP_DIM__();
	void __DEBUG_MAP_SEED__();
	void __DEBUG_MAP_PRINT__();
};

