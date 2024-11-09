#include "Map.h"

Map::Map()
{
	//Setting pseudo-random seed using chrono
	m_seed = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	srand(m_seed);

	//Random generation of map dimentions
	m_mapWidth = rand() % (kMaxMapWidth - kMinMapWidth) + kMinMapWidth;
	//Generating random width size untill length is larger
	while ((m_mapHeight = rand() % (kMaxMapHeight - kMminMapHeight) + kMminMapHeight) >= m_mapWidth) {}

	m_matrix.resize(m_mapHeight);
	for (std::vector<Object*>& line : m_matrix) {
		line.resize(m_mapWidth);
	}

	__DEBUG_MAP__();
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



void Map::__DEBUG_MAP__() {
	std::cout << "Width: " << (int)m_mapWidth << "\nHeight:" << (int)m_mapHeight << "\n"; //FOR DEBUGGING
	for (auto l : m_matrix) { for (auto c : l) std::cout << "x "; std::cout << "\n"; }
}