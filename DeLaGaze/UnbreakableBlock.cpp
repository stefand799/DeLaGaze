#include "UnbreakableBlock.h"

void UnbreakableBlock::Render(){
}

void UnbreakableBlock::Print() const{
	std::cout << "\033[37;44m" << " " << "\033[0m"; //U
}

bool UnbreakableBlock::CanMoveHere(int i, int j){
	return false;
}
