#include "BreakableBlock.h"

BreakableBlock::BreakableBlock(std::pair<size_t, size_t> pos) :
	Wall{pos}
{
}

void BreakableBlock::Print() const{
	std::cout << "\033[37;43m" << " " << "\033[0m"; //B
}

void BreakableBlock::OnBreak(){
	//Possibly call for break animation
}

crow::json::wvalue BreakableBlock::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "BreakableBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}
