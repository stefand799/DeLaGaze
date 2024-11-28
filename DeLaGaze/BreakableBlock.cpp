#include "BreakableBlock.h"

void BreakableBlock::Render(){
}

void BreakableBlock::Print() const{
	std::cout << "\033[37;43m" << " " << "\033[0m"; //B
}

void BreakableBlock::OnBreak(){
}

bool BreakableBlock::CanMoveHere(int i, int j){
	return false;
}
