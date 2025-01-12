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
	for (int i = 0; i < m_mapHeight; i++) {
		crow::json::wvalue rowJson;
		rowJson["type"] = ObjectTypeToString(i, i);
		rowJson["x"] = i;
		rowJson["y"] = i;
		mapJson.push_back(std::move(rowJson));
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
	if (std::chrono::duration<float>(now - m_lastShrinkTime) < kShrinkCooldown)
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

bool Map::Generate(const std::vector<uint8_t>& probabilities, uint32_t seed)
{
	//Ensuring there is no second call of generate
	if (m_isGenerated) return false;

	m_lastShrinkTime = Clock::now();

	//Initializing objects
	m_probabilities = probabilities;
	m_seed = seed;
	m_generator.seed(seed);

	if (!VerifyProbabilities()) {
		std::cerr << "Probabilities provided for map generation are invalid.\n";
		return false;
	}

	// Generating
	GenerateDimensions();
	GenerateStructures();

	// Verifing if there are any isolated players and solving paths between players accordingly

	BreakUnbreakableOnBestPath(FindBestPath({ 0,0 }, { m_mapWidth - 1,m_mapHeight - 1 }), { 0,0 }, { m_mapWidth - 1,m_mapHeight - 1 });
	BreakUnbreakableOnBestPath(FindBestPath({ m_mapWidth - 1 ,0 }, { 0 ,m_mapHeight - 1 }), { m_mapWidth - 1 ,0 }, { 0 ,m_mapHeight - 1 });

	m_isGenerated = true;
	return true;

}



bool Map::VerifyProbabilities() {
	if (m_probabilities.size() != 3) return false;

	int sumOfProbabilities = 0;
	for (int it : m_probabilities) sumOfProbabilities += it;
	return sumOfProbabilities == 100;

}

void Map::GenerateDimensions() {

	// Defining distributions for width
	std::uniform_int_distribution<> widthDist(kMinMapWidth, kMaxMapWidth);
	m_mapWidth = widthDist(m_generator); // Generate width within range

	// Generate height to ensure m_mapHeight <= m_mapWidth
	std::uniform_int_distribution<> heightDist(kMinMapHeight, std::min(kMaxMapHeight, m_mapWidth));
	m_mapHeight = heightDist(m_generator);

	// Resize and initialize the matrix
	m_matrix.resize(m_mapHeight, std::vector<std::shared_ptr<Object>>(m_mapWidth));
}

bool Map::GenerateStructures() {
	// Defining a vector for all breakable blocks for the future generation of bombtraps
	std::vector<std::shared_ptr<Object>*> breakableBlocksVector;

	// Defining distrubutions for random object generation
	std::uniform_int_distribution<> objectRandomNumber(0, 99);
	// Generating random objects according to probabilities
	for (size_t y = 0; y < m_matrix.size(); ++y) {
		std::vector<std::shared_ptr<Object>>& line = m_matrix[y];
		for (size_t x = 0; x < m_matrix[0].size(); ++x) {
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
						breakableBlocksVector.push_back(&object); //TODO:::::::::::::::::::::MAKE THIS AS WEAK_PTR(OBJECT)?
						//breakableBlocksVector.push_back(reinterpret_cast<BreakableBlock**>(&object));////////////////////////////////////////////////////////////

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

	// Making sure the corners are paths;
	MakeCornerPathway(0, 0, breakableBlocksVector);
	MakeCornerPathway(m_mapWidth - 1, 0, breakableBlocksVector);
	MakeCornerPathway(0, m_mapHeight - 1, breakableBlocksVector);
	MakeCornerPathway(m_mapWidth - 1, m_mapHeight - 1, breakableBlocksVector);

	// Transform a random number of BreakableBlocks generated into BombTraps
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
	// Defining distrubutions for random bombtrap generation
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
	// bestPath both holds the solution (each position's values is its parent) and serves as a closedList (storing visited positions)
	// Initial closedList (the size of map m_matrix has {-1,-1} on all positions)
	std::vector<std::vector<std::pair<size_t, size_t>>> bestPath{ m_mapHeight, std::vector<std::pair<size_t,size_t>>{m_mapWidth, {-1,-1}} };

	// openList is the queue of Positions to visit
	std::priority_queue<BestPathNode, std::vector<BestPathNode>, std::greater<BestPathNode>> openList;

	// Relative directions to look for neighbours
	int dx[4]{ -1, 1, 0 , 0 };
	int dy[4]{ 0, 0 , 1, -1 };

	// Placing the start position in the queue
	openList.push(BestPathNode(start, 0, 0));

	// Dijkstra
	while (!openList.empty()) {
		BestPathNode current = openList.top();
		openList.pop();
		auto [x, y] = current.GetPosition(); //Binding structure
		for (int k = 0; k < 4; ++k) { //Looking for all 4 neighbours
			size_t nextX = x + dx[k];
			size_t nextY = y + dy[k];
			if (nextX < 0 || nextY < 0 || nextX >= m_mapWidth || nextY >= m_mapHeight) continue; // invalid position
			if (bestPath[nextY][nextX] != std::make_pair<size_t, size_t>(-1, -1)) continue; // position already visited
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





void Map::__DEBUG_MAP_DIM__() {
	std::cout << "Width: " << (int)m_mapWidth << "\nHeight:" << (int)m_mapHeight << std::endl; //FOR DEBUGGING ONLY
	for (auto l : m_matrix) { for (auto c : l) std::cout << "x "; std::cout << "\n"; }
}

void Map::__DEBUG_MAP_SEED__() {
	std::cout << "Seed:" << m_seed << std::endl;
}

void Map::__DEBUG_MAP_PRINT__() {
	for (std::vector<std::shared_ptr<Object>>& line : m_matrix) {
		for (std::shared_ptr<Object>& object : line) {
			object->Print();
			std::cout << " ";
		}
		std::cout << "\n";
	}
}
