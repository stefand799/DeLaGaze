#include <numeric>
#include "Map.h"
#include "Player.h"
#include "DeadlyBlock.h"

Map::Map() :
	m_isGenerated{false},
	m_seed{0},
	m_mapHeight{0},
	m_mapWidth{0},
	m_shrinkOrder{0}
{
}

crow::json::wvalue Map::toJson()
{
	crow::json::wvalue::list mapJson;
	for (size_t y : std::views::iota(0,static_cast<int>(m_mapHeight))) {
		for (size_t x : std::views::iota(0,static_cast<int>(m_mapWidth))) {
			crow::json::wvalue rowJson;
			rowJson["type"] = ObjectTypeToString(y, x);
			rowJson["x"] = x;
			rowJson["y"] = y;
			mapJson.push_back(std::move(rowJson));
		}
	}
	crow::json::wvalue mapData;
	mapData["height"] = m_mapHeight;
	mapData["width"] = m_mapWidth;
	mapData["map"] = std::move(mapJson);

	return mapData;
}

std::string Map::ObjectTypeToString(int i, int j)
{
	std::shared_ptr<Object> obj = m_matrix[i][j];
	Object::ObjectType type = obj->GetType();
	switch (type) {
	case Object::ObjectType::Player:
		return "Player";
	case Object::ObjectType::Bullet:
		return "Bullet";
	case Object::ObjectType::Pathway:
		return "Pathway";
	case Object::ObjectType::UnbreakableBlock:
		return "UnbreakableBlock";
	case Object::ObjectType::BreakableBlock:
		return "BreakableBlock";
	case Object::ObjectType::BombTrapBlock:
		return "BombTrapBlock";
	default:
		return "Unknown ObjectType";
	}
}

const std::vector<std::shared_ptr<Object>>& Map::operator[](size_t line) const
{
	return m_matrix[line];
}


std::vector<std::shared_ptr<Object>>& Map::operator[](size_t line)
{
	return m_matrix[line];
}



void Map::Shrink()
{
	if (m_isGenerated == false) return;

	Clock::time_point now = Clock::now();
	if (std::chrono::duration<float>(now - m_lastShrinkTime) < (m_shrinkOrder==0? std::chrono::seconds(3) : kShrinkCooldown))
		return;
	if (m_shrinkOrder * 2 > std::min(m_mapWidth, m_mapHeight)) 
		return;

	m_lastShrinkTime = now;
	for (int8_t x = m_shrinkOrder; x < m_mapWidth - m_shrinkOrder; x++) {

		if (std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(m_matrix[m_shrinkOrder][x])) {
			player->OnDeath();
		}
		m_matrix[m_shrinkOrder][x] = std::make_shared<DeadlyBlock>(std::pair<size_t, size_t>{x, m_shrinkOrder});

		if (std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(m_matrix[m_mapHeight - 1 - m_shrinkOrder][x])) {
			player->OnDeath();
		}
		m_matrix[m_mapHeight - 1 - m_shrinkOrder][x] = std::make_shared<DeadlyBlock>(std::pair<size_t, size_t>{x, m_mapHeight - 1 - m_shrinkOrder});
	}

	for (int8_t y = m_shrinkOrder; y < m_mapHeight - m_shrinkOrder; y++) {

		if (std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(m_matrix[y][m_shrinkOrder])) {
			player->OnDeath();
		}
		m_matrix[y][m_shrinkOrder] = std::make_shared<DeadlyBlock>(std::pair<size_t, size_t>{m_shrinkOrder, y});

		if (std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(m_matrix[y][m_mapWidth - 1 - m_shrinkOrder])) {
			player->OnDeath();
		}
		m_matrix[y][m_mapWidth - 1 - m_shrinkOrder] = std::make_shared<DeadlyBlock>(std::pair<size_t, size_t>{m_mapWidth - 1 - m_shrinkOrder, y});
	}

	m_shrinkOrder++;
}

void Map::InitiateShrinking()
{
	m_lastShrinkTime = Clock::now();
}

bool Map::Generate(const std::vector<uint8_t>& probabilities, uint32_t seed)
{
	if (m_isGenerated) return false;

	m_lastShrinkTime = Clock::now();

	m_probabilities = probabilities;
	m_seed = seed;
	m_generator.seed(seed);

	if (!VerifyProbabilities()) {
		std::cerr << "Probabilities provided for map generation are invalid.\n";
		return false;
	}

	GenerateDimensions();
	GenerateStructures();

	BreakUnbreakableOnBestPath(FindBestPath({ 0,0 }, { m_mapWidth - 1,m_mapHeight - 1 }), { 0,0 }, { m_mapWidth - 1,m_mapHeight - 1 });
	BreakUnbreakableOnBestPath(FindBestPath({ m_mapWidth - 1 ,0 }, { 0 ,m_mapHeight - 1 }), { m_mapWidth - 1 ,0 }, { 0 ,m_mapHeight - 1 });

	m_isGenerated = true;
	return true;

}



bool Map::VerifyProbabilities() {
	if (m_probabilities.size() != 3) return false;

	return std::accumulate(m_probabilities.begin(), m_probabilities.end(), 0) == 100;

}

void Map::GenerateDimensions() {

	std::uniform_int_distribution<> widthDist(kMinMapWidth, kMaxMapWidth);
	m_mapWidth = widthDist(m_generator);

	std::uniform_int_distribution<> heightDist(kMinMapHeight, std::min(kMaxMapHeight, m_mapWidth));
	m_mapHeight = heightDist(m_generator);

	m_matrix.resize(m_mapHeight, std::vector<std::shared_ptr<Object>>(m_mapWidth));
}

bool Map::GenerateStructures() {
	std::vector<std::shared_ptr<Object>*> breakableBlocksVector;

	std::uniform_int_distribution<> objectRandomNumber(0, 99);
	for (size_t y : std::views::iota(0, static_cast<int>(m_mapHeight))) {
		std::vector<std::shared_ptr<Object>>& line = m_matrix[y];
		for (size_t x : std::views::iota(0, static_cast<int>(m_mapWidth))) {
			std::shared_ptr<Object>& object = line[x];
			uint8_t sum = 0;
			uint8_t current = 0;
			uint8_t randomNumber = objectRandomNumber(m_generator);
			for (; sum < 100 && current < m_probabilities.size(); ++current) {
				if (randomNumber >= sum && randomNumber < sum + m_probabilities[current]) {
					switch (current) {
					case 0:
						object = std::make_shared<Pathway>(std::pair<size_t, size_t>{x, y});
						break;
					case 1:
						object = std::make_shared<UnbreakableBlock>(std::pair<size_t, size_t>{x, y});
						break;
					case 2:
						object = std::make_shared<BreakableBlock>(std::pair<size_t, size_t>{x, y});
						breakableBlocksVector.push_back(&object); 

						break;
					default:
						std::cerr << "Could not initialize position!";
						return false;
					}
					break;
				}
				sum += m_probabilities[current];
			}
		}
	}

	MakeCornerPathway(0, 0, breakableBlocksVector);
	MakeCornerPathway(m_mapWidth - 1, 0, breakableBlocksVector);
	MakeCornerPathway(0, m_mapHeight - 1, breakableBlocksVector);
	MakeCornerPathway(m_mapWidth - 1, m_mapHeight - 1, breakableBlocksVector);

	PlaceBombs(breakableBlocksVector);


	return true;
}

void Map::MakeCornerPathway(size_t x, size_t y, std::vector<std::shared_ptr<Object>*>& breakableBlocksVector) {
	if (!std::dynamic_pointer_cast<Pathway>(m_matrix[y][x])) {
		auto it = std::find(breakableBlocksVector.begin(), breakableBlocksVector.end(), &(m_matrix[y][x]));
		if (it != breakableBlocksVector.end()) {
			breakableBlocksVector.erase(it);
		}
		m_matrix[y][x] = std::make_shared<Pathway>(std::pair<size_t, size_t>{x, y});
	}
}

void Map::PlaceBombs(std::vector<std::shared_ptr<Object>*>& breakableBlocksVector)
{
	std::uniform_int_distribution<> bombRandomNumber(1, kTotalBombCount);
	size_t bombCount = bombRandomNumber(m_generator);
	if (bombCount >= breakableBlocksVector.size()) {
		for (std::shared_ptr<Object>* curr : breakableBlocksVector) {
			std::shared_ptr<BreakableBlock> breakableBlock = std::static_pointer_cast<BreakableBlock>(*curr);
			auto [x, y] = breakableBlock->GetPos();
			(*curr) = std::make_shared<BombTrapBlock>( std::pair<size_t,size_t>{x,y}, this );
		}
	}
	else {
		std::uniform_int_distribution<> bombRandomPosition(0, breakableBlocksVector.size() - 1);
		while (bombCount > 0) {
			size_t bombPos = bombRandomPosition(m_generator);
			if (!std::dynamic_pointer_cast<BombTrapBlock>(*(breakableBlocksVector[bombPos]))) {
				std::shared_ptr<BreakableBlock> breakableBlock = std::static_pointer_cast<BreakableBlock>(*(breakableBlocksVector[bombPos]));
				auto [x, y] = breakableBlock->GetPos();
				*(breakableBlocksVector[bombPos]) = std::make_shared<BombTrapBlock>(std::pair<size_t,size_t>{x,y}, this);
				--bombCount;
			}
		}
	}
}


std::vector<std::vector<std::pair<size_t, size_t>>> Map::FindBestPath(std::pair<size_t, size_t> start, std::pair<size_t, size_t> end)
{
	std::vector<std::vector<std::pair<size_t, size_t>>> bestPath{ m_mapHeight, std::vector<std::pair<size_t,size_t>>{m_mapWidth, {-1,-1}} };

	std::priority_queue<BestPathNode, std::vector<BestPathNode>, std::greater<BestPathNode>> openList;

	int dx[4]{ -1, 1, 0 , 0 };
	int dy[4]{ 0, 0 , 1, -1 };

	openList.push(BestPathNode(start, 0, 0));

	while (!openList.empty()) {
		BestPathNode current = openList.top();
		openList.pop();
		auto [x, y] = current.GetPosition();
		for (int k = 0; k < 4; ++k) {
			size_t nextX = x + dx[k];
			size_t nextY = y + dy[k];
			if (nextX < 0 || nextY < 0 || nextX >= m_mapWidth || nextY >= m_mapHeight) continue;
			if (bestPath[nextY][nextX] != std::make_pair<size_t, size_t>(-1, -1)) continue;
			if (std::dynamic_pointer_cast<UnbreakableBlock>(m_matrix[nextY][nextX])) {
				openList.push(BestPathNode{ {nextX,nextY}, current.GetNormalBlocksCount(), current.GetUnbreakableBlocksCount() + 1 });
			}
			else {
				openList.push(BestPathNode{ {nextX,nextY}, current.GetNormalBlocksCount() + 1, current.GetUnbreakableBlocksCount() });
			}
			bestPath[nextY][nextX] = current.GetPosition();
			if (std::pair<size_t, size_t>{nextX, nextY} == end) return bestPath;
		}
	}
	return bestPath;
}

void Map::BreakUnbreakableOnBestPath(std::vector<std::vector<std::pair<size_t, size_t>>> path, std::pair<size_t, size_t> start, std::pair<size_t, size_t> end)
{
	std::pair<size_t,size_t> curr = end;
	while (curr != start) {
		auto [x, y] = curr;
		if (std::dynamic_pointer_cast<UnbreakableBlock>(m_matrix[y][x])) {
			m_matrix[y][x] = std::make_shared<BreakableBlock>( std::pair<size_t,size_t>{x,y} );
		}
		curr = path[y][x];
	}
}


bool Map::IsWithinBounds(const int& i, const int& j) const
{
	return i >= 0 && j >= 0 && i < m_mapHeight && j < m_mapWidth;
}

