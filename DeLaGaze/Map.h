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
public:
	Map();
	Map(const Map&) = default;
	Map(Map&&) = delete;
	crow::json::wvalue toJson();
	std::string ObjectTypeToString(int i, int j);
	~Map() = default;


	const std::vector<std::shared_ptr<Object>>& operator[](size_t line) const;
	
	std::vector<std::shared_ptr<Object>>& operator[](size_t line);

	uint8_t GetMapWidth() const { return m_mapWidth; };
	uint8_t GetMapHeight() const { return m_mapHeight; };

public:

	void Shrink();
	void InitiateShrinking();

	bool Generate(
		const std::vector<uint8_t>& probabilities = std::vector<uint8_t>{
			35, 
			25, 
			40  },
			uint32_t seed = std::random_device{}()
			);


private:
	bool VerifyProbabilities();
	void GenerateDimensions();
	bool GenerateStructures();
	void MakeCornerPathway(size_t x, size_t y, std::vector<std::shared_ptr<Object>*>& breakableBlocksVector);
	void PlaceBombs(std::vector<std::shared_ptr<Object>*>& breakableBlocksVector);

	

	std::vector<std::vector<std::pair<size_t, size_t>>> FindBestPath(std::pair<size_t, size_t> start, std::pair<size_t,size_t> end);
	void BreakUnbreakableOnBestPath(std::vector<std::vector<std::pair<size_t, size_t>>> path, std::pair<size_t, size_t> start, std::pair<size_t, size_t> end);

private:
	using Clock = std::chrono::high_resolution_clock;

	class BestPathNode {
	public:
		BestPathNode(std::pair<size_t, size_t> pos, uint32_t normalBlocksCount, uint32_t unbreakableBlocksCount) :
			m_pos{ pos },
			m_normalBlocksCount {normalBlocksCount},
			m_unbreakableBlocksCount{unbreakableBlocksCount} {}
		bool operator>(const BestPathNode& other) const {
			if (this->m_unbreakableBlocksCount != other.m_unbreakableBlocksCount) {
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



private:
	const uint8_t kMinMapWidth { 12 }, kMinMapHeight { 8 },
		kMaxMapWidth { 20 }, kMaxMapHeight  { 12 };
	const uint8_t kTotalBombCount{ 3 };

	const std::chrono::seconds kShrinkCooldown = std::chrono::seconds(15);

private:
	std::vector<std::vector<std::shared_ptr<Object>>> m_matrix;
	std::vector<uint8_t> m_probabilities;
	uint8_t m_mapWidth, m_mapHeight;
	uint32_t m_seed;
	std::mt19937 m_generator;

	Clock::time_point m_lastShrinkTime;
	int8_t m_shrinkOrder;

	bool m_isGenerated : 1;

public:
	bool IsWithinBounds(const int& i, const int& j) const;
};

