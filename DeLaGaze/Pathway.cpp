#include "Pathway.h"

void Pathway::Render(){
}

void Pathway::Print() const{
	std::cout << "\033[37;42m" << " " << "\033[0m"; //P
}

bool Pathway::CanMoveHere(int i, int j){
	return true;
}
