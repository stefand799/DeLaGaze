#include "DeadlyBlock.h"

DeadlyBlock::DeadlyBlock(std::pair<size_t, size_t> pos) : UnbreakableBlock{ pos }
{

}



crow::json::wvalue DeadlyBlock::toJson()
{
    crow::json::wvalue jsonObj;
	jsonObj["type"] = "DeadlyBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}

bool DeadlyBlock::CanMoveHere()
{
    return true;
}
