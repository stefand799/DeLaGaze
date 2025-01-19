#include "Pathway.h"

Pathway::Pathway(std::pair<size_t, size_t> pos) :
	Block{pos}
{
}



crow::json::wvalue Pathway::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "Pathway";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}

bool Pathway::CanMoveHere(){
	return true;
}
