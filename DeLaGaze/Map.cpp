#include "Map.h"



const std::vector<Object*>& Map::operator[](size_t line) const
{
	return m_matrix[line];
}
std::vector<Object*>& Map::operator[](size_t line)
{
	return m_matrix[line];
}

bool Map::generate(const std::vector<Object*>& objects, const std::vector<uint8_t>& probabilities, uint32_t seed)
{
	//Initializing objects
	m_objects = objects;
	m_probabilities = probabilities;
	m_seed = seed;
	m_generator.seed(seed);

	if (!verifyObjectsAndProbabilities()) {
		std::cerr << "Objects and/or probabilities provided for map generation are invalid.\n";
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



bool Map::verifyObjectsAndProbabilities()
{
	if (m_objects.size() < 3) return false;
	if (m_probabilities.size() != m_objects.size()) return false;

	if (!(dynamic_cast<Pathway*>(m_objects[0]))) return false;
	if (!(dynamic_cast<UnbreakableBlock*>(m_objects[1]))) return false; // if pathway will remain unbreakable block, this will always not work
	if (!(dynamic_cast<BreakableBlock*>(m_objects[2]))) return false;

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
	// Defining distrubutions for random object generation
	std::uniform_int_distribution<> objectRandomNumber(0, 99);
	// Generating random objects according to probabilities
	for (std::vector<Object*>& line : m_matrix) {
		for (Object*& object : line) {
			uint8_t sum = 0;
			uint8_t current = 0;
			uint8_t randomNumber = objectRandomNumber(m_generator);
			for (; sum < 100 && current < m_objects.size(); ++current) {
				if (randomNumber >= sum && randomNumber < sum + m_probabilities[current]) {
					object = m_objects[current];
					break;
				}
				sum += m_probabilities[current];
			}
		}
	}

	// Making sure the corners are paths;
	m_matrix[0][0] = m_objects[0];
	m_matrix[0][m_mapWidth-1] = m_objects[0];
	m_matrix[m_mapHeight-1][0] = m_objects[0];
	m_matrix[m_mapHeight-1][m_mapWidth-1] = m_objects[0];

	// TODO: Generate a random number of bombs by saving the breakable walls in an vector like so std::vector<Object**>???? and changing
	// random points to bombs according to the random number generated, a randomly generated(1 to 3 bombs) number of times 

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
