#include "BreakableBlock.h"

BreakableBlock::BreakableBlock(std::pair<int, int> pos) :
	Wall{pos}
{
}

void BreakableBlock::Render(){
}

void BreakableBlock::Print() const{
	std::cout << "\033[37;43m" << " " << "\033[0m"; //B
}

void BreakableBlock::OnBreak(){
	//Possibly call for break animation
}

bool BreakableBlock::CanMoveHere(int i, int j){
	return false;
}

crow::json::wvalue BreakableBlock::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "BreakableBlock";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}
