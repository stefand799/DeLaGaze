#include "Pathway.h"

Pathway::Pathway(std::pair<size_t, size_t> pos) :
	Block{pos}
{
}

void Pathway::Render(){
}

void Pathway::Print() const{
	std::cout << "\033[37;42m" << " " << "\033[0m"; //P
}

crow::json::wvalue Pathway::toJson()
{
	crow::json::wvalue jsonObj;
	jsonObj["type"] = "Pathway";
	jsonObj["x"] = m_pos.first;
	jsonObj["y"] = m_pos.second;
	return jsonObj;
}

bool Pathway::CanMoveHere(int i, int j){
	return true;
}
