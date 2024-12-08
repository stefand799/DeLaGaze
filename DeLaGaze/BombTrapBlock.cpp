#include "BombTrapBlock.h"
#include "Map.h"
#include "Player.h"


BombTrapBlock::BombTrapBlock(std::pair<size_t, size_t> pos, Map* map) :
	BreakableBlock{ pos },
	m_map{ map },
	m_hasExploded{ false }
{
}

void BombTrapBlock::Render() {
}

void BombTrapBlock::Print() const {
	std::cout << "\033[30;43m" << "*" << "\033[0m"; //X
}

void BombTrapBlock::OnBreak() {
	//Possibly add boom animation
	if (!m_hasExploded) {
		m_hasExploded = true;
		Boom();
	}
}

bool BombTrapBlock::HasExploded()
{
	return m_hasExploded;
}

crow::json::wvalue BombTrapBlock::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "BombTrapBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}

void BombTrapBlock::Boom()
{
	size_t xBorderLeft = std::max((int)m_pos.first - m_boomRadius, 0);
	size_t xBorderRight = std::min((int)m_pos.first + m_boomRadius, m_map->GetMapWidth() - 1);
	size_t yBorderTop = std::max((int)m_pos.second - m_boomRadius, 0);
	size_t yBorderBottom = std::min((int)m_pos.second + m_boomRadius, m_map->GetMapHeight() - 1);

	for (size_t y = yBorderTop; y <= yBorderBottom; ++y) {
		for (size_t x = xBorderLeft; x <= xBorderRight; ++x) {
			if (x == m_pos.first && y == m_pos.second) continue;
			if (!(*m_map)[y][x]) continue;
			if (BombTrapBlock* bomb = dynamic_cast<BombTrapBlock*>((*m_map)[y][x])) {
				if (bomb->HasExploded()) continue;
			}
			if (BreakableBlock* breakable = dynamic_cast<BreakableBlock*>((*m_map)[y][x])) {
				breakable->OnBreak();
				if((*m_map)[y][x]) delete (*m_map)[y][x];
				(*m_map)[y][x] = new Pathway{{x,y}};
			}
		}
	}

}

