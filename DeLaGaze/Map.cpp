#include "Map.h"



Map::~Map()
{
	for (std::vector<Object*>& line : m_matrix) {
		for (Object*& object : line) {
			if (object) delete object;
		}
	}
}

const std::vector<Object*>& Map::operator[](size_t line) const
{
	return m_matrix[line];
}
std::vector<Object*>& Map::operator[](size_t line)
{
	return m_matrix[line];
}

bool Map::generate(const std::vector<uint8_t>& probabilities, uint32_t seed)
{
	//Initializing objects
	m_probabilities = probabilities;
	m_seed = seed;
	m_generator.seed(seed);

	if (!verifyProbabilities()) {
		std::cerr << "Probabilities provided for map generation are invalid.\n";
		return false;
	}

	//Generating
	generateDimensions();
	generateStructures();

	// TODO: Verify if there are any isolated spaces and make paths accordingly

	//__DEBUG_MAP_SEED__();
	//__DEBUG_MAP_DIM__();
	__DEBUG_MAP_PRINT__();

	return true;

}



bool Map::verifyProbabilities() {
	if (m_probabilities.size() != 3) return false;

	int sumOfProbabilities = 0;
	for (int it : m_probabilities) sumOfProbabilities += it;
	if (sumOfProbabilities > 100 || sumOfProbabilities < 100 || sumOfProbabilities < 0) return false;

	return true;
}

void Map::generateDimensions() {

	// Defining distributions for width
	std::uniform_int_distribution<> widthDist(kMinMapWidth, kMaxMapWidth);
	m_mapWidth = widthDist(m_generator); // Generate width within range

	// Generate height to ensure m_mapHeight <= m_mapWidth
	std::uniform_int_distribution<> heightDist(kMinMapHeight, std::min(kMaxMapHeight, m_mapWidth));
	m_mapHeight = heightDist(m_generator);

	// Resize and initialize the matrix
	m_matrix.resize(m_mapHeight, std::vector<Object*>(m_mapWidth));
}

bool Map::generateStructures() {
	// Defining a vector for all breakable blocks for the future generation of bombtraps
	std::vector<Object**> breakableBlocksVector;

	// Defining distrubutions for random object generation
	std::uniform_int_distribution<> objectRandomNumber(0, 99);
	// Generating random objects according to probabilities
	for (std::vector<Object*>& line : m_matrix) {
		for (Object*& object : line) {
			uint8_t sum = 0;
			uint8_t current = 0;
			uint8_t randomNumber = objectRandomNumber(m_generator);
			for (; sum < 100 && current < m_probabilities.size(); ++current) {
				if (randomNumber >= sum && randomNumber < sum + m_probabilities[current]) {
					switch (current) {
					case 0:
						object = new Pathway;
						break;
					case 1:
						object = new UnbreakableBlock;
						break;
					case 2:
						object = new BreakableBlock;
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

	// Making sure the corners are paths;
	if (!dynamic_cast<Pathway*>(m_matrix[0][0])) {
		delete m_matrix[0][0];
		m_matrix[0][0] = new Pathway;
	}
	if (!dynamic_cast<Pathway*>(m_matrix[0][m_mapWidth - 1])) {
		delete m_matrix[0][m_mapWidth - 1];
		m_matrix[0][m_mapWidth - 1] = new Pathway;
	}
	if (!dynamic_cast<Pathway*>(m_matrix[m_mapHeight - 1][0])) {
		delete m_matrix[m_mapHeight - 1][0];
		m_matrix[m_mapHeight - 1][0] = new Pathway;
	}
	if (!dynamic_cast<Pathway*>(m_matrix[m_mapHeight - 1][m_mapWidth - 1])) {
		delete m_matrix[m_mapHeight - 1][m_mapWidth - 1];
		m_matrix[m_mapHeight - 1][m_mapWidth - 1] = new Pathway;
	}

	
	// Defining distrubutions for random bombtrap generation
	std::uniform_int_distribution<> bombRandomNumber(1, kTotalBombCount);
	size_t bombCount = bombRandomNumber(m_generator);
	if (bombCount >= breakableBlocksVector.size()) {
		for (Object** curr : breakableBlocksVector) {
			delete *curr;
			*curr = new BombTrapBlock;
		}
	}
	else {
		std::uniform_int_distribution<> bombRandomPosition(0, breakableBlocksVector.size());
		while (bombCount > 0) {
			size_t bombPos = bombRandomPosition(m_generator);
			if (!dynamic_cast<BombTrapBlock*>(*(breakableBlocksVector[bombPos]))) {
				delete *(breakableBlocksVector[bombPos]);
				*(breakableBlocksVector[bombPos]) = new BombTrapBlock;
				--bombCount;
			}
		}
	}
	



	return true;
}






void Map::__DEBUG_MAP_DIM__() {
	std::cout << "Width: " << (int)m_mapWidth << "\nHeight:" << (int)m_mapHeight << std::endl; //FOR DEBUGGING
	for (auto l : m_matrix) { for (auto c : l) std::cout << "x "; std::cout << "\n"; }
}

void Map::__DEBUG_MAP_SEED__() {
	std::cout << "Seed:" << m_seed << std::endl;
}

void Map::__DEBUG_MAP_PRINT__() {
	for (std::vector<Object*>& line : m_matrix) {
		for (Object*& object : line) {
			object->print();
			std::cout << " ";
		}
		std::cout << "\n";
	}
}
