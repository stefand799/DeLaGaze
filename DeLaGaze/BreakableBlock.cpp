#include "BreakableBlock.h"

BreakableBlock::BreakableBlock(std::pair<size_t, size_t> pos) :
	Wall{pos}
{
}



void BreakableBlock::OnBreak(){
}

crow::json::wvalue BreakableBlock::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "BreakableBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}
