#include "Map.h"

Map::Map(uint32_t seed /*= std::random_device{}()*/) :
	m_seed {seed},
	m_generator {seed}
{

	__DEBUG_MAP_SEED__();
}

const std::vector<Object*>& Map::operator[](size_t line) const
{
	std::cout << "const\n";
	return m_matrix[line];
}
std::vector<Object*>& Map::operator[](size_t line)
{
	std::cout << "not const\n";
	return m_matrix[line];
}

bool Map::generate(const std::vector<Object*>& objects)
{
	//Initializing objects
	m_objects = objects;
	
	generateDimensions();


	__DEBUG_MAP_DIM__();

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




void Map::__DEBUG_MAP_DIM__() {
	std::cout << "Width: " << (int)m_mapWidth << "\nHeight:" << (int)m_mapHeight << std::endl; //FOR DEBUGGING
	for (auto l : m_matrix) { for (auto c : l) std::cout << "x "; std::cout << "\n"; }
}

void Map::__DEBUG_MAP_SEED__()
{
	std::cout << "Seed:" << m_seed << std::endl;
}
