#include "BombTrapBlock.h"

void BombTrapBlock::Render(){
}

void BombTrapBlock::Print() const{
	std::cout << "\033[30;43m" << "*" << "\033[0m"; //X
}

void BombTrapBlock::OnBreak(){
}

crow::json::wvalue BombTrapBlock::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "BombTrapBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}
