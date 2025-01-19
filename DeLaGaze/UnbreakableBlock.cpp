#include "UnbreakableBlock.h"

UnbreakableBlock::UnbreakableBlock(std::pair<size_t, size_t> pos) :
	Wall{pos}
{
}



crow::json::wvalue UnbreakableBlock::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "UnbreakableBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}
