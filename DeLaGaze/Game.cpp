#include "Game.h"

Game::Game()
{
	//Setting pseudo-random seed using chrono
	m_seed = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	srand(m_seed);

	//Random generation of map dimentions
	m_mapLength = rand() % (kMaxMapLength - kMinMapLength) + kMinMapLength;
	//Generating random width size untill length is larger
	while ((m_mapWidth = rand() % (kMaxMapWidth - kMminMapWidth) + kMminMapWidth) >= m_mapLength) {}

	m_map.resize(m_mapWidth);
	for (std::vector<Object*>& line : m_map) {
		line.resize(m_mapLength);
	}

	//__DEBUG_MAP__();
}

Game::~Game()
{
}

void Game::__DEBUG_MAP__() {
	std::cout << "Length: " << (int)m_mapLength << "\nWidth:  " << (int)m_mapWidth << "\n"; //FOR DEBUGGING
	for (auto l : m_map) { for (auto c : l) std::cout << "x "; std::cout << "\n"; }
}